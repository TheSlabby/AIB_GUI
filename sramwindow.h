#ifndef SRAMWINDOW_H
#define SRAMWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QVBoxLayout>


class sramwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit sramwindow(QWidget *parent = nullptr);
    void populateTable(const std::unordered_map<uint32_t, uint8_t> &sramMemory);

private:
    QWidget *centralWidget;
    QVBoxLayout *layout;
    QTableWidget *table;
};

#endif // SRAMWINDOW_H
