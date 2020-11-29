#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTextEdit>


class Absolute : public QWidget {
public:
    Absolute(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* edit{new QTextEdit{this}};
        edit->setGeometry(5, 5, 200, 150);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    
    Absolute window;
    window.setWindowTitle("Absolute");
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
