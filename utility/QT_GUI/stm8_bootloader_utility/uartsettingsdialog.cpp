#include "uartsettingsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSerialPortInfo>

UartSettingsDialog::UartSettingsDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("UART Configuration");
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 1. Available Ports Section
    QGroupBox *portGroupBox = new QGroupBox("Connection Port", this);
    QFormLayout *portFormLayout = new QFormLayout(portGroupBox);

    // CRITICAL FIX: Instantiate the combo box object FIRST!
    portCombo = new QComboBox(portGroupBox);

    // Automatically scan the machine's hardware ports with specific filters
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    bool physicalPortFound = false;

    for (const QSerialPortInfo &port : ports) {
        QString name = port.portName().toLower();
        QString desc = port.description().toLower();
        QString manufacturer = port.manufacturer().toLower();

        // Skip known virtual/cluttered system interfaces on Linux
        if (name.contains("ttymxc") || name.contains("ttyama") ||
            (name.contains("ttys") && !name.contains("ttyusb") && !name.contains("ttyacm")) ||
            name.contains("gserial")) {
            continue;
        }

        // Identify microcontroller interfaces (USB-Serial adapters, ST-Link, CH340, CP210x, FTDI)
        bool isTargetDevice = name.contains("usb") || name.contains("acm") ||
                              desc.contains("serial") || desc.contains("stlink") ||
                              manufacturer.contains("ftdi") || manufacturer.contains("ch34x");

        QString displayName = port.portName() + " (" + port.description() + ")";

        if (isTargetDevice) {
            portCombo->insertItem(0, displayName + " ⭐", port.portName());
            physicalPortFound = true;
        } else {
            portCombo->addItem(displayName, port.portName());
        }
    }

    // Handle fallbacks safely now that portCombo is valid
    if (portCombo->count() == 0) {
        portCombo->addItem("No active ports found!");
        portCombo->setEnabled(false);
    } else if (physicalPortFound) {
        portCombo->setCurrentIndex(0);
    }

    portFormLayout->addRow("Select Port:", portCombo);
    mainLayout->addWidget(portGroupBox);

    // 2. Baud Rate Radio Buttons
    QGroupBox *baudGroupBox = new QGroupBox("Baud Rate", this);
    QVBoxLayout *baudLayout = new QVBoxLayout(baudGroupBox);
    baud9600 = new QRadioButton("9600", baudGroupBox);
    baud115200 = new QRadioButton("115200", baudGroupBox);
    baud115200->setChecked(true);
    baudLayout->addWidget(baud9600);
    baudLayout->addWidget(baud115200);

    baudGroup = new QButtonGroup(this);
    baudGroup->addButton(baud9600, 9600);
    baudGroup->addButton(baud115200, 115200);
    mainLayout->addWidget(baudGroupBox);

    // 3. Form Layout for Data Bits, Stop Bits, Parity
    QGroupBox *otherGroupBox = new QGroupBox("Port Settings", this);
    QFormLayout *formLayout = new QFormLayout(otherGroupBox);

    dataBitsCombo = new QComboBox(otherGroupBox);
    dataBitsCombo->addItems({"5", "6", "7", "8"});
    dataBitsCombo->setCurrentText("8");

    stopBitsCombo = new QComboBox(otherGroupBox);
    stopBitsCombo->addItems({"1", "1.5", "2"});
    stopBitsCombo->setCurrentText("1");

    parityCombo = new QComboBox(otherGroupBox);
    parityCombo->addItems({"No Parity", "Even Parity", "Odd Parity"});

    formLayout->addRow("Data Bits:", dataBitsCombo);
    formLayout->addRow("Stop Bits:", stopBitsCombo);
    formLayout->addRow("Parity:", parityCombo);
    mainLayout->addWidget(otherGroupBox);

    // 4. Action Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Save", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

UartSettings UartSettingsDialog::getSettings() const {
    UartSettings settings;
    settings.portName = portCombo->currentData().toString();
    settings.baudRate = baudGroup->checkedId();

    int db = dataBitsCombo->currentText().toInt();
    if (db == 5)  settings.dataBits = QSerialPort::Data5;
    else if (db == 6) settings.dataBits = QSerialPort::Data6;
    else if (db == 7) settings.dataBits = QSerialPort::Data7;
    else              settings.dataBits = QSerialPort::Data8;

    QString sb = stopBitsCombo->currentText();
    if (sb == "1")       settings.stopBits = QSerialPort::OneStop;
    else if (sb == "1.5") settings.stopBits = QSerialPort::OneAndHalfStop;
    else                 settings.stopBits = QSerialPort::TwoStop;

    int parityIdx = parityCombo->currentIndex();
    if (parityIdx == 1)      settings.parity = QSerialPort::EvenParity;
    else if (parityIdx == 2) settings.parity = QSerialPort::OddParity;
    else                     settings.parity = QSerialPort::NoParity;

    return settings;
}
