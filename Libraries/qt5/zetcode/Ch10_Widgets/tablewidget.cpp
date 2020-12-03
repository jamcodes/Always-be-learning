#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QTableWidget>

class Table : public QWidget
{
public:
    Table(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* hbox{new QHBoxLayout{this}};
        auto* table{new QTableWidget{25, 25, this}};
        hbox->addWidget(table);
        setLayout(hbox);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    Table window;
    window.resize(300, 200);
    window.setWindowTitle("Table");
    window.show();

    return app.exec();
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
