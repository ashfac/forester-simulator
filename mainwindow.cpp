#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"

#include <QLabel>
#include <QMessageBox>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , m_MainWindow(new Ui::MainWindow)
    , m_Status(new QLabel)
    , m_Console(new Console)
    , m_Settings(new SettingsDialog)
    , m_Serial(new QSerialPort(this))
    , m_SubaruForester(new SubaruForester())
{
    m_MainWindow->setupUi(this);
    m_Console->setEnabled(false);
    setCentralWidget(m_Console);

    m_MainWindow->actionConnect->setEnabled(true);
    m_MainWindow->actionDisconnect->setEnabled(false);
    m_MainWindow->actionQuit->setEnabled(true);
    m_MainWindow->actionConfigure->setEnabled(true);

    m_MainWindow->statusBar->addWidget(m_Status);

    initActionsConnections();

    connect(m_Serial, &QSerialPort::errorOccurred, this, &MainWindow::handleErrorRx);
    connect(m_Serial, &QSerialPort::readyRead, this, &MainWindow::onSerialReceived);
    connect(m_Console, &Console::getData, this, &MainWindow::writeToSerial);

    openSerialPort();
    sendToSubaruForester("ATZ\r");
}

MainWindow::~MainWindow()
{
    delete m_SubaruForester;
    delete m_Serial;
    delete m_Settings;
    delete m_MainWindow;
}

void MainWindow::openSerialPort()
{
    const SettingsDialog::Settings p = m_Settings->settings();
    m_Serial->setPortName("COM1");
//    m_Serial->setPortName(p.name);
    m_Serial->setBaudRate(p.baudRate);
    m_Serial->setDataBits(p.dataBits);
    m_Serial->setParity(p.parity);
    m_Serial->setStopBits(p.stopBits);
    m_Serial->setFlowControl(p.flowControl);

    //TODO: Create a Folder Selection item on Settings Form
    const QString logDirectory = "C:/Users/CAASAHM/Documents/developement/obd/subaruforester/logs/";

    if (m_Serial->open(QIODevice::ReadWrite)) {
        m_Console->setEnabled(true);
        m_Console->setLocalEchoEnabled(p.localEchoEnabled);
        m_MainWindow->actionConnect->setEnabled(false);
        m_MainWindow->actionDisconnect->setEnabled(true);
        m_MainWindow->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(m_Serial->portName(), p.stringBaudRate, p.stringDataBits, p.stringParity, p.stringStopBits, p.stringFlowControl));

//        m_LogFile = new QFile(logDirectory + QDateTime::currentDateTime().toString("[yyyy-MM-dd_hhmmss]") + " Qt.log");

        m_LogFile = new QFile(logDirectory + "debug.log");

        if(m_LogFile->open(QIODevice::WriteOnly | QFile::Text | QFile::Append | QFile::Unbuffered)) {
            //TODO: Get the Data Link Device type from the Settings Form
            m_SubaruForester->get_dlc()->connect(
                        dlc::device_t::obdlink,
                        std::bind(&MainWindow::onDataLinkReceived, this, std::placeholders::_1));

        } else {
            QMessageBox::critical(this, tr("Error"), m_LogFile->errorString());

            showStatusMessage(tr("Error creating log file"));
        }
    } else {
        QMessageBox::critical(this, tr("Error"), m_Serial->errorString());

        showStatusMessage(tr("Error opening serial port"));
    }
}

void MainWindow::closeSerialPort()
{
    m_SubaruForester->get_dlc()->disconnect();

    if (m_Serial->isOpen())
        m_Serial->close();

    m_LogFile->close();

    m_Console->setEnabled(false);
    m_MainWindow->actionConnect->setEnabled(true);
    m_MainWindow->actionDisconnect->setEnabled(false);
    m_MainWindow->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

void MainWindow::writeToSerial(const QByteArray &data)
{
    m_Serial->write(data);
    logData(data);
}

void MainWindow::onSerialReceived()
{
    QByteArray data = m_Serial->readAll();

    sendToSubaruForester(data);
    logData(data);
}

void MainWindow::sendToSubaruForester(const QByteArray &data)
{
    m_SubaruForester->get_dlc()->send(data.toStdString());
}

void MainWindow::onDataLinkReceived(const std::string &data)
{
    writeToSerial(QByteArray::fromStdString(data));
}

void MainWindow::logData(const QByteArray data)
{
    m_Console->putData(data);
    m_LogFile->write(data);
    m_LogFile->flush();
}

void MainWindow::handleErrorRx(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_Serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::initActionsConnections()
{
    connect(m_MainWindow->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(m_MainWindow->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(m_MainWindow->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(m_MainWindow->actionConfigure, &QAction::triggered, m_Settings, &SettingsDialog::show);
    connect(m_MainWindow->actionClear, &QAction::triggered, m_Console, &Console::clear);
    connect(m_MainWindow->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(m_MainWindow->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_Status->setText(message);
}
