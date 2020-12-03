#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>


class LineEdit : public QWidget
{
public:
    LineEdit(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* name{new QLabel{"Name:", this}};
        name->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        auto* age{new QLabel{"Age:", this}};
        age->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        auto* occupation{new QLabel{"Occupation:", this}};
        occupation->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        auto* edt_name{new QLineEdit{this}};
        auto* edt_age{new QLineEdit{this}};
        auto* edt_occupation{new QLineEdit{this}};

        auto* grid{new QGridLayout{}};

        grid->addWidget(name, 0, 0, 1, 1);
        grid->addWidget(edt_name, 0, 1, 1, 1);
        grid->addWidget(age, 1, 0, 1, 1);
        grid->addWidget(edt_age, 1, 1, 1, 1);
        grid->addWidget(occupation, 2, 0, 1, 1);
        grid->addWidget(edt_occupation, 2, 1, 1, 1);

        setLayout(grid);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    LineEdit window;
    window.setWindowTitle("LineEdit");
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
