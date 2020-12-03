#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFrame>
#include <QSplitter>
#include <QHBoxLayout>


class Splitter : public QWidget
{
public:
    Splitter(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* hbox{new QHBoxLayout{this}};

        auto* fr_topleft{new QFrame{this}};
        fr_topleft->setFrameShape(QFrame::StyledPanel);

        auto* fr_topright{new QFrame{this}};
        fr_topright->setFrameShape(QFrame::StyledPanel);

        auto* splitter1{new QSplitter{Qt::Horizontal, this}};
        splitter1->addWidget(fr_topleft);
        splitter1->addWidget(fr_topright);

        auto* fr_bottom{new QFrame{this}};
        fr_bottom->setFrameShape(QFrame::StyledPanel);

        auto* splitter2{new QSplitter{Qt::Vertical, this}};
        splitter2->addWidget(splitter1);
        splitter2->addWidget(fr_bottom);

        QList<int> sizes{{50, 100}};
        splitter2->setSizes(sizes);

        hbox->addWidget(splitter2);
        setLayout(hbox);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    Splitter window;
    window.setWindowTitle("Splitter");
    window.resize(300, 200);
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
