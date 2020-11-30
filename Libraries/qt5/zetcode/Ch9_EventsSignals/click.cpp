#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QHBoxLayout>


class Click : public QWidget
{
public:
    Click(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* btn_quit{new QPushButton{"Quit", this}};
        auto* hbox{new QHBoxLayout{this}};
        hbox->setSpacing(5);
        hbox->addWidget(btn_quit, 0, Qt::AlignLeft | Qt::AlignTop);

        // The `connect` member function connects signal to the slot
        connect(btn_quit, &QPushButton::clicked, qApp, &QApplication::quit);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Click window;
    window.setWindowTitle("Click");
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
