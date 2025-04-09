#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QDebug>
#include "sramwindow.h"

#pragma pack(push, 1)
struct AIBData {
    uint16_t preamble;
    float temperature;
    char msg[64];

    // sram stuff
    uint32_t sram_address;
    uint8_t sram_data;
};
#pragma pack(pop)


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;
    QByteArray buffer;

    AIBData aibData;
    std::unordered_map<uint32_t, uint8_t> sramMemory;
    sramwindow *sramWin = nullptr;



private slots:
    void readSerialData();

};
#endif // MAINWINDOW_H
