#include "sramwindow.h"

sramwindow::sramwindow(QWidget *parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    layout(new QVBoxLayout),
    table(new QTableWidget)
{
    setWindowTitle("SRAM Viewer");
    resize(800, 500);
    setCentralWidget(centralWidget);
    layout->addWidget(table);
    centralWidget->setLayout(layout);
}

void sramwindow::populateTable(const std::unordered_map<uint32_t, uint8_t> &sramMemory) {
    table->clear();
    table->setRowCount(static_cast<int>(sramMemory.size()));
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(QStringList() << "Address" << "Data" << "Note");
    table->setColumnWidth(0, 200);
    table->setColumnWidth(1, 200);
    table->setColumnWidth(2, 365);

    int row = 0;
    for (const auto&[address, value] : sramMemory) {
        // memory addr label
        QString addressString = QString("0x%1").arg(address, 8, 16, QLatin1Char( '0' ));
        QTableWidgetItem *addrItem = new QTableWidgetItem(addressString);
        addrItem->setFlags(addrItem->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 0, addrItem);

        // memory value
        QString dataString = QString("0x%1").arg(value, 8, 16, QLatin1Char( '0' ));
        QTableWidgetItem *dataItem = new QTableWidgetItem(dataString);
        dataItem->setFlags(dataItem->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 1, dataItem);

        // TODO define note
        QString noteString = QString("Temperature Reading");
        QTableWidgetItem *noteItem = new QTableWidgetItem(noteString);
        noteItem->setFlags(noteItem->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 2, noteItem);


        ++row;
    }
}
