#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QList>
#include <QString>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QGridLayout>
#include <QPushButton>


class Calculator : public QWidget
{
public:
    Calculator(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* grid{new QGridLayout{this}};
        grid->setSpacing(2);

        const QList<QString> values{{
            "7", "8", "9", "/",
            "4", "5", "6", "*",
            "1", "2", "3", "-",
            "0", ".", "=", "+"
        }};

        int pos{0};
        for (int i{0}; i != 4; ++i) {
            for (int j{0}; j != 4; ++j) {
                auto* btn{new QPushButton{values[pos], this}};
                btn->setFixedSize(40, 40);
                grid->addWidget(btn, i, j);
                ++pos;
            }
        }

        setLayout(grid);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Calculator window;
    window.move(300, 300);
    window.setWindowTitle("Calculator");
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
