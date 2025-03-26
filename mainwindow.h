#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QDebug>


#pragma pack(push, 1)
struct Heartbeat {
    uint16_t preamble;
    uint8_t id;
    float temperature;
    char msg[64];
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
    Heartbeat aibData;

private slots:
    void readSerialData();

};
#endif // MAINWINDOW_H
