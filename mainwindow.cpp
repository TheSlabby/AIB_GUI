#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup uart
    serial.setPortName("/dev/ttyACM0");
    serial.setBaudRate(QSerialPort::Baud115200);     // Adjust if needed
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    
    if (serial.open(QIODevice::ReadOnly)) {
        qDebug() << "Sucess";
        connect(&serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    } else {
        qDebug() << "Couldn't open serial";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSerialData() {
    
    // QByteArray data = serial.readAll();
    buffer.append(serial.readAll());
    // ui->uartRaw->setPlainText(QString::fromUtf8(buffer));  // or .insertPlainText(text);

    // look for preamble (find start of aibData packet)
    int index = -1;
    for (int i = 0; i < buffer.size() - 2; ++i) {
        if ((uint8_t)buffer[i] == 0x55 && (uint8_t)buffer[i+1] == 0xAA) {
            index = i;
            // qDebug() << "PREAMBLE FOUND at index: " << index;
            break;
        }
    }

    if (index == -1) {
        qDebug() << "preamble not found";
    } else if (buffer.size() - index >= sizeof(Heartbeat)) {
        QByteArray packet = buffer.mid(index, sizeof(Heartbeat)); // basically substring
        /// set aibData based on this packet
        memcpy(&aibData, packet.constData(), sizeof(Heartbeat));

        QString msg = aibData.msg;

        qDebug() << "Got message | temp: " << aibData.temperature << ", msg: " << aibData.msg;

        // buffer.remove(0, index + sizeof(Heartbeat));
        buffer.clear();

        ui->progressBar->setValue(aibData.temperature);
        ui->uartRaw->append("Incoming Message: " + msg);
    }


}