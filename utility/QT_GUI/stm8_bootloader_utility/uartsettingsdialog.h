#ifndef UARTSETTINGDIALOG_H
#define UARTSETTINGDIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QSerialPort> // Required for native port types

// This struct now holds native QSerialPort configuration types
struct UartSettings {
    QString portName;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity parity;
};

class UartSettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit UartSettingsDialog(QWidget *parent = nullptr);
    UartSettings getSettings() const;

private:
    QComboBox *portCombo; // Dropdown to list available ports
    QButtonGroup *baudGroup;
    QRadioButton *baud9600;
    QRadioButton *baud115200;
    QComboBox *dataBitsCombo;
    QComboBox *stopBitsCombo;
    QComboBox *parityCombo;
};

#endif