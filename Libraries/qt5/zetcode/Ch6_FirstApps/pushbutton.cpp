#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QPushButton>


class MyButton : public QWidget {
public:
    MyButton(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* quit_btn{new QPushButton{"Quit", this}};
        quit_btn->setGeometry(50, 40, 75, 30);
        // qApp is a global pointer to the application object
        connect(quit_btn, &QPushButton::clicked, qApp, &QApplication::quit);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    MyButton window;
    window.resize(250, 150);
    window.setWindowTitle("QPushButton");
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
