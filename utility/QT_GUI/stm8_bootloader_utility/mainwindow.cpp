#include "mainwindow.h"
#include "uartsettingsdialog.h"
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QAction>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QStatusBar>
#include <QDebug>
#include <QFile>
#include <QThread>
#include <QtMath> // CRITICAL: Required for qLn() block calculations

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_serialPort(nullptr)
{
    // 1. Properly set up the Menu Bar using the window's built-in instance
    QMenuBar *menu = this->menuBar();
    QMenu *fileMenu = new QMenu("&File", this);
    QMenu *settingMenu = new QMenu("&Setting", this);

    QAction *quitAction = new QAction("&Quit", this);
    QAction *uartSetting = new QAction("&UART setting", this);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    fileMenu->addAction(quitAction);
    settingMenu->addAction(uartSetting);

    menu->addMenu(fileMenu);
    menu->addMenu(settingMenu);

    // 2. Create the central container widget
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // 3. Create and add child widgets to layout
    QPushButton *fileButton = new QPushButton("Choose Firmware File...", centralWidget);
    QPushButton *button1 = new QPushButton("Connect", centralWidget);
    QPushButton *button2 = new QPushButton("Flash", centralWidget);
    QPushButton *button3 = new QPushButton("Disconnect", centralWidget);

    layout->addWidget(fileButton);
    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);

    // 4. Handle the file chooser click event using a Lambda function
    connect(fileButton, &QPushButton::clicked, this, [this]() {
        this->m_firmwarePath = QFileDialog::getOpenFileName(
            this,
            "Select STM8 Hex/Bin File",
            "",
            "Hex Files (*.hex);;Binary Files (*.bin);;All Files (*)",
            nullptr,
            QFileDialog::DontUseNativeDialog
            );

        if (!this->m_firmwarePath.isEmpty()) {
            qDebug() << "Successfully tracked firmware file:" << this->m_firmwarePath;
        }
    });

    // 5. Connect the UART settings click event (CLEANED DUPES)
    connect(uartSetting, &QAction::triggered, this, [this]() {
        UartSettingsDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            UartSettings config = dialog.getSettings();

            if (config.portName.isEmpty()) return;

            QString parityText = "None";
            if (config.parity == QSerialPort::EvenParity) parityText = "Even";
            else if (config.parity == QSerialPort::OddParity) parityText = "Odd";

            QString statusMsg = QString("Port: %1 | Baud: %2 | Bits: %3 | Stop: %4 | Parity: %5")
                                    .arg(config.portName)
                                    .arg(config.baudRate)
                                    .arg(config.dataBits == QSerialPort::Data8 ? "8" : QString::number(config.dataBits))
                                    .arg(config.stopBits == QSerialPort::OneStop ? "1" : "2")
                                    .arg(parityText);

            m_statusLabel->setText(statusMsg);
            this->m_uartConfig = config;
        }
    });

    // Map Flash button to executeBootloader
    connect(button2, &QPushButton::clicked, this, &MainWindow::executeBootloader);

    this->setCentralWidget(centralWidget);

    // Initialize the permanent label widget at the bottom right
    m_statusLabel = new QLabel("UART: Not Configured", this);
    this->statusBar()->addPermanentWidget(m_statusLabel);
}

quint8 MainWindow::crc8Update(quint8 data, quint8 crc) {
    crc ^= data;
    for (int i = 0; i < 8; ++i) {
        if (crc & 0x80) {
            crc = (crc << 1) ^ 0x07;
        } else {
            crc <<= 1;
        }
    }
    return crc & 0xFF;
}

quint8 MainWindow::calculateFileCrc(const QString &filePath, int blockSize) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return 0;

    quint8 crc = 0x0;
    while (!file.atEnd()) {
        QByteArray chunk = file.read(blockSize);
        if (chunk.size() < blockSize) {
            chunk.append(QByteArray(blockSize - chunk.size(), (char)0xFF));
        }
        for (char byte : chunk) {
            crc = crc8Update(static_cast<quint8>(byte), crc);
        }
    }
    return crc;
}

void MainWindow::executeBootloader() {
    const int BLOCK_SIZE = 64;

    if (m_firmwarePath.isEmpty()) {
        this->statusBar()->showMessage("Error: No firmware file selected.");
        return;
    }
    if (m_uartConfig.portName.isEmpty()) {
        this->statusBar()->showMessage("Error: UART interface not configured.");
        return;
    }

    QFile file(m_firmwarePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open firmware file.";
        return;
    }

    // Initialize Port connection configuration
    m_serialPort = new QSerialPort(this);
    m_serialPort->setPortName(m_uartConfig.portName);
    m_serialPort->setBaudRate(m_uartConfig.baudRate);
    m_serialPort->setDataBits(m_uartConfig.dataBits);
    m_serialPort->setStopBits(m_uartConfig.stopBits);
    m_serialPort->setParity(m_uartConfig.parity);

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port:" << m_serialPort->errorString();
        return;
    }

    this->statusBar()->showMessage("Resetting STM8 target hardware...");

    // Replicate exactly the Python script DTR sequence logic
    m_serialPort->setDataTerminalReady(true);
    QThread::msleep(100);
    m_serialPort->setDataTerminalReady(false);
    QThread::msleep(1000); // Wait for the microbootloader core to initialize

    // Calculate firmware file size and block arrays metadata requirements
    qint64 fileSize = file.size();
    int totalChunks = static_cast<int>(qCeil(static_cast<double>(fileSize) / BLOCK_SIZE));
    quint8 fileCrc = calculateFileCrc(m_firmwarePath, BLOCK_SIZE);

    qDebug() << "Total file size:" << fileSize << "bytes | Transmitting:" << totalChunks << "chunks.";

    // Build the entrance request payload package (REQ_ENTER matching Python structural payload)
    QByteArray request;
    request.append(static_cast<char>(0xDE));
    request.append(static_cast<char>(0xAD));
    request.append(static_cast<char>(0xBE));
    request.append(static_cast<char>(0xEF));
    request.append(static_cast<char>(totalChunks));
    request.append(static_cast<char>(fileCrc));
    request.append(static_cast<char>(fileCrc)); // Duplicated CRC matches your python payload append script execution array line exactly

    m_serialPort->write(request);
    m_serialPort->flush();

    // Clear raw local computer input buffers before reading target device responses
    m_serialPort->clear(QSerialPort::Input);

    // Read the bootloader validation confirmation signature response (expecting 4 bytes)
    if (m_serialPort->waitForReadyRead(2000)) {
        QByteArray rxBuff = m_serialPort->read(4);
        qDebug() << "Bootloader Handshake Init Response:" << rxBuff.toHex();
    } else {
        qDebug() << "Timeout waiting for initial bootloader response.";
        m_serialPort->close();
        file.close();
        return;
    }

    int currentChunk = 0;
    this->statusBar()->showMessage("Flashing firmware payload...");

    // Step through each block chunk and write out data stream down the connection wire loops
    while (!file.atEnd()) {
        // Step A: The target microcontroller sends 2 sync validation bytes BEFORE accepting each packet chunk loop block data stream
        if (!m_serialPort->waitForReadyRead(2000)) {
            qDebug() << "Timeout waiting for chip ready response at chunk:" << currentChunk;
            break;
        }

        QByteArray readySignal = m_serialPort->read(2);
        // Print out the sync bytes from the STM8 device to verify active hardware tracking state
        qDebug() << "Chunk" << currentChunk << "Sync Code received:" << readySignal.toHex();

        // Step B: Read file slice array chunk and fill trailing gaps with padding bytes matching Python logic patterns
        QByteArray chunk = file.read(BLOCK_SIZE);
        if (chunk.size() < BLOCK_SIZE) {
            chunk.append(QByteArray(BLOCK_SIZE - chunk.size(), (char)0xFF));
        }

        // Step C: Push block into the serial pipeline link instance
        m_serialPort->write(chunk);
        m_serialPort->flush();
        currentChunk++;
    }

    // Step D: Evaluate final script verification matching ACK / NACK payload verification structures
    if (m_serialPort->waitForReadyRead(2000)) {
        QByteArray ack = m_serialPort->read(2);
        qDebug() << "Final Verification Response Code:" << ack.toHex();

        if (ack.size() >= 2 && static_cast<quint8>(ack[0]) == 0xAA && static_cast<quint8>(ack[1]) == 0xBB) {
            qDebug() << "Flashing Status: Success!";
            this->statusBar()->showMessage("Firmware Flash Successful!");
        } else if (ack.size() >= 2 && static_cast<quint8>(ack[0]) == 0xDE && static_cast<quint8>(ack[1]) == 0xAD) {
            qDebug() << "Flashing Status: CRC Mismatch Failure!";
            this->statusBar()->showMessage("Flash Failed: CRC Checksum Mismatch.");
        } else {
            qDebug() << "Flashing Status: Invalid target validation response.";
            this->statusBar()->showMessage("Flash Failed: Unknown hardware response validation signature.");
        }
    } else {
        qDebug() << "Timeout waiting for final transmission completion ACK.";
        this->statusBar()->showMessage("Flash Failed: Transmission confirmation timeout.");
    }

    m_serialPort->close();
    file.close();
}

MainWindow::~MainWindow() = default;
