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
    QMenu *aboutMenu = new QMenu("&About", this);

    QAction *quitAction = new QAction("&Quit", this);
    QAction *uartSetting = new QAction("&UART setting", this);
    QAction *aboutAction = new QAction("&About", this);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    fileMenu->addAction(quitAction);
    settingMenu->addAction(uartSetting);
    aboutMenu->addAction(aboutAction);

    menu->addMenu(fileMenu);
    menu->addMenu(settingMenu);
    menu->addMenu(aboutMenu);

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

    // NEW: Initialize and style the diagnostic Log Window
    m_logWindow = new QPlainTextEdit(centralWidget);
    m_logWindow->setReadOnly(true); // Prevents the user from typing inside it

    // Style it to look like a terminal window
    m_logWindow->setStyleSheet(
        "QPlainTextEdit {"
        "   background-color: #1e1e1e;" // Dark charcoal background
        "   color: #00ff00;"            // Classic terminal green text
        "   font-family: 'Courier New', monospace;"
        "   font-size: 11pt;"
        "}"
        );

    // Add a placeholder indicator text
    m_logWindow->appendPlainText("[SYSTEM LOG INITIALIZED]");

    // Add the log window directly to the main vertical layout stretch
    layout->addWidget(m_logWindow);

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
        m_logWindow->appendPlainText("ERROR: Failed to open firmware file.");
        return;
    }

    // ==========================================
    // FIXED: Move port allocation to the TOP
    // ==========================================
    if (!m_serialPort) {
        m_serialPort = new QSerialPort(this);
    }

    m_serialPort->setPortName(m_uartConfig.portName);
    m_serialPort->setBaudRate(m_uartConfig.baudRate);
    m_serialPort->setDataBits(m_uartConfig.dataBits);
    m_serialPort->setStopBits(m_uartConfig.stopBits);
    m_serialPort->setParity(m_uartConfig.parity);

    if (!m_serialPort->isOpen() && !m_serialPort->open(QIODevice::ReadWrite)) {
        m_logWindow->appendPlainText("ERROR: Failed to open port: " + m_serialPort->errorString());
        file.close();
        return;
    }

    m_logWindow->appendPlainText("\n--- Starting Flashing Sequence ---");
    this->statusBar()->showMessage("Resetting STM8 target hardware...");

    // Now this call is completely safe from crashing!
    m_serialPort->setDataTerminalReady(true);
    QThread::msleep(100);
    m_serialPort->setDataTerminalReady(false);

    // Give the STM8 ample time to finish resetting and outputting hardware noise
    QThread::msleep(300);

    // Read and completely discard the initial hardware startup bytes (0x00 0x80... noise)
    if (m_serialPort->bytesAvailable() > 0) {
        QByteArray startupNoise = m_serialPort->readAll();
        m_logWindow->appendPlainText(QString("Discarded startup hardware noise: %1")
                                         .arg(QString(startupNoise.toHex(' ').toUpper())));
    }

    // Clear internal software line registers to guarantee a completely clean slate
    m_serialPort->clear(QSerialPort::Input);

    qint64 fileSize = file.size();
    int totalChunks = static_cast<int>(qCeil(static_cast<double>(fileSize) / BLOCK_SIZE));
    quint8 fileCrc = calculateFileCrc(m_firmwarePath, BLOCK_SIZE);

    // Build the 7-byte request preamble package (REQ_ENTER)
    QByteArray request;
    request.append(static_cast<char>(0xDE));
    request.append(static_cast<char>(0xAD));
    request.append(static_cast<char>(0xBE));
    request.append(static_cast<char>(0xEF));
    request.append(static_cast<char>(totalChunks));
    request.append(static_cast<char>(fileCrc));
    request.append(static_cast<char>(fileCrc));

    // Log raw bytes sent for Preamble
    m_logWindow->appendPlainText(QString("TX -> Raw Bytes: %1").arg(QString(request.toHex(' ').toUpper())));

    m_serialPort->write(request);
    m_serialPort->flush();

    // Read the Handshake Response safely using a strict buffer accumulation loop
    QByteArray rxBuff;
    int expectedHandshakeBytes = 4;
    int timeoutMs = 2000;
    int elapsedMs = 0;

    while (rxBuff.size() < expectedHandshakeBytes && elapsedMs < timeoutMs) {
        if (m_serialPort->waitForReadyRead(100)) {
            rxBuff.append(m_serialPort->readAll());
        } else {
            elapsedMs += 100;
        }
    }

    // Log what was actually caught after cleaning the noise out
    // if (rxBuff.size() >= expectedHandshakeBytes) {
    //     m_logWindow->appendPlainText(QString("RX <- Clean Preamble Response: %1 (%2)")
    //                                      .arg(QString(rxBuff.toHex(' ').toUpper()))
    //                                      .arg(QString(rxBuff)));
    // } else {
    //     m_logWindow->appendPlainText("RX <- ERROR: Handshake Timeout or incomplete packet received!");
    //     m_serialPort->close();
    //     file.close();
    //     return;
    // }

    // Clear input buffer before starting block loop to prevent tracking errors
    m_serialPort->clear(QSerialPort::Input);
    int currentChunk = 0;
    this->statusBar()->showMessage("Streaming firmware blocks...");

    // Stream all payload chunks back-to-back blindly
    while (!file.atEnd()) {
        QByteArray chunk = file.read(BLOCK_SIZE);
        if (chunk.size() < BLOCK_SIZE) {
            chunk.append(QByteArray(BLOCK_SIZE - chunk.size(), (char)0xFF));
        }

        m_serialPort->write(chunk);
        m_serialPort->flush();

        // 1. Verify "boot" text signature response from target
            if (m_serialPort->waitForReadyRead(2000)) {
            QByteArray rxBuff = m_serialPort->readAll();
            m_logWindow->appendPlainText(QString("RX <- Target Handshake: %1 (%2)")
                                             .arg(QString(rxBuff.toHex(' ').toUpper()))
                                             .arg(QString(rxBuff)));
        } else {
            m_logWindow->appendPlainText("RX <- ERROR: Handshake Timeout! Connection rejected.");
            m_serialPort->close();
            return;
        }

        // Log the exact raw payload block bytes sent down the wire
        m_logWindow->appendPlainText(QString("TX -> Block %1/%2 Raw Bytes:\n%3")
                                         .arg(currentChunk + 1)
                                         .arg(totalChunks)
                                         .arg(QString(chunk.toHex(' ').toUpper())));

        QApplication::processEvents(); // Forces UI thread log display refresh instantly
        QThread::msleep(100); // Clear target RAM write cycles
        currentChunk++;
    }

    m_logWindow->appendPlainText("Transmission complete. Awaiting final MCU validation check...");
    this->statusBar()->showMessage("Verifying final CRC check validation...");

    // Evaluate final ACK/NACK status verification code from the controller
    if (m_serialPort->waitForReadyRead(2000)) { // Increased slightly to accommodate chip process delay
        QByteArray ack = m_serialPort->readAll();

        // Log the raw confirmation response bytes
        m_logWindow->appendPlainText(QString("RX <- Final Verification Code (Raw Hex): %1").arg(QString(ack.toHex(' ').toUpper())));

        if (ack.size() >= 2 && static_cast<quint8>(ack[0]) == 0xAA && static_cast<quint8>(ack[1]) == 0xBB) {
            m_logWindow->appendPlainText("RESULT: SUCCESS! [ACK Received (AA BB)] -> Firmware flashed and verified.");
            this->statusBar()->showMessage("Firmware Flash Successful!");
        } else if (ack.size() >= 2 && static_cast<quint8>(ack[0]) == 0xDE && static_cast<quint8>(ack[1]) == 0xAD) {
            m_logWindow->appendPlainText("RESULT: FAILED! [NACK Received (DE AD)] -> CRC Checksum Mismatch.");
            this->statusBar()->showMessage("Flash Failed: CRC Checksum Mismatch.");
        } else {
            m_logWindow->appendPlainText("RESULT: FAILED! Invalid target validation signature sequence received.");
            this->statusBar()->showMessage("Flash Failed: Invalid hardware validation.");
        }
    } else {
        m_logWindow->appendPlainText("RX <- ERROR: Confirmation Timeout waiting for final validation ACK/NACK.");
        this->statusBar()->showMessage("Flash Failed: Confirmation Timeout.");
    }

    m_serialPort->close();
    file.close();
}

MainWindow::~MainWindow() = default;
