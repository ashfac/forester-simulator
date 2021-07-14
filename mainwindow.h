#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QFile>
#include <QTextStream>

#include "subaruforester.h"

QT_BEGIN_NAMESPACE

class QLabel;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeToSerial(const QByteArray &data);
    void onSerialReceived();

    void sendToSubaruForester(const QByteArray &data);
    void onDataLinkReceived(const std::string &data);

    void logData(const QByteArray data);

    void handleErrorRx(QSerialPort::SerialPortError error);

private:
    void initActionsConnections();

private:
    void showStatusMessage(const QString &message);

    Ui::MainWindow *m_MainWindow = nullptr;
    QLabel *m_Status = nullptr;
    Console *m_Console = nullptr;
    SettingsDialog *m_Settings = nullptr;

    QSerialPort *m_Serial = nullptr;
    QFile *m_LogFile = nullptr;

    SubaruForester *m_SubaruForester = nullptr;

};

#endif // MAINWINDOW_H
