#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>


class Layouts : public QWidget
{
public:
    Layouts(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* vbox{new QVBoxLayout{}};
        auto* hbox{new QHBoxLayout{this}};

        auto* lw{new QListWidget{this}};
        lw->addItem("The Omen");
        lw->addItem("The Exorcist");
        lw->addItem("Notes on a Scandal");
        lw->addItem("Fargo");
        lw->addItem("Capote");

        auto* btn_add{new QPushButton{"Add", this}};
        auto* btn_rename{new QPushButton{"Rename", this}};
        auto* btn_remove{new QPushButton{"Remove", this}};
        auto* btn_removeall{new QPushButton{"Remove All", this}};

        vbox->setSpacing(3);
        vbox->addStretch(1);
        vbox->addWidget(btn_add);
        vbox->addWidget(btn_rename);
        vbox->addWidget(btn_remove);
        vbox->addWidget(btn_removeall);
        vbox->addStretch(1);

        hbox->addWidget(lw);
        hbox->addSpacing(15);
        hbox->addLayout(vbox);

        setLayout(hbox);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Layouts window;
    window.resize(350, 250);
    window.setWindowTitle("Layout nesting");
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
