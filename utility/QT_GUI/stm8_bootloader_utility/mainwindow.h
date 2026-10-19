#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPlainTextEdit>
#include <QSerialPort>      // For holding the port connection state
#include "uartsettingsdialog.h" // For recognizing UartSettings struct

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    // Protocol specific helper functions
    quint8 crc8Update(quint8 data, quint8 crc);
    quint8 calculateFileCrc(const QString &filePath, int blockSize);
    void executeBootloader();

private:
    QPlainTextEdit *m_logWindow;
    QLabel *m_statusLabel;
    QString m_firmwarePath;     // Remembers the chosen file path
    UartSettings m_uartConfig;  // Remembers the configured baud/parity settings
    QSerialPort *m_serialPort;  // Active connection interface to the STM8 chip
};
#endif // MAINWINDOW_H
