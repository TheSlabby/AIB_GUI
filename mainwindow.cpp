#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup uart
    serial.setPortName("COM5");
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

    // show sram memory window
    connect(ui->sramMemoryButton, &QPushButton::clicked, this, [=]() {
        if (!sramWin) {
            sramWin = new sramwindow(this);
        }

        // test sram data
        sramMemory[0x01] = 0x34;
        sramMemory[0x02] = 0x67;

        sramWin->populateTable(sramMemory);
        sramWin->show();
    });

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

    if (index == -1 || buffer.size() - index < sizeof(AIBData))
        return;

    QByteArray packet = buffer.mid(index, sizeof(AIBData)); // basically substring
    /// set aibData based on this packet
    memcpy(&aibData, packet.constData(), sizeof(AIBData));

    QString msg = aibData.msg;

    // qDebug() << "Got message | temp: " << aibData.temperature << ", msg: " << aibData.msg;
    qDebug().noquote() << QString::asprintf("Address: 0x%08X | Data: 0x%02X", aibData.sram_address, aibData.sram_data);

    buffer.remove(0, index + sizeof(AIBData));
    // buffer.clear();

    ui->progressBar->setValue(aibData.temperature);
    ui->uartRaw->append("Incoming Message: " + msg);

    // add sram memory to our local map
    sramMemory[aibData.sram_address] = aibData.sram_data;

}
