#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>


class Buttons : public QWidget {
public:
    Buttons(QWidget* parent = nullptr)
        : QWidget{parent},
          btn_ok_{new QPushButton{"OK", this}},
          btn_apply_{new QPushButton{"Apply", this}}
    {
        auto* vbox{new QVBoxLayout{this}};
        auto* hbox{new QHBoxLayout{}};

        // params: child, stretch-factor, alignment
        hbox->addWidget(btn_ok_, 1, Qt::AlignRight);
        hbox->addWidget(btn_apply_, 0);

        vbox->addStretch(1);
        vbox->addLayout(hbox);
    }

private:
    QPushButton* btn_ok_;
    QPushButton* btn_apply_;
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Buttons window;
    window.resize(290, 170);
    window.setWindowTitle("Buttons");
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
