#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QFrame>
#include <QGridLayout>


class Cursors : public QWidget {
public:
    Cursors(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        QFrame* frame1{new QFrame{this}};
        frame1->setFrameStyle(QFrame::Box);
        frame1->setCursor(Qt::SizeAllCursor);

        QFrame* frame2{new QFrame{this}};
        frame2->setFrameStyle(QFrame::Box);
        frame2->setCursor(Qt::WaitCursor);

        QFrame* frame3{new QFrame{this}};
        frame3->setFrameStyle(QFrame::Box);
        frame3->setCursor(Qt::PointingHandCursor);

        QGridLayout* grid{new QGridLayout{this}};
        grid->addWidget(frame1, 0, 0);
        grid->addWidget(frame2, 0, 1);
        grid->addWidget(frame3, 0, 2);

        setLayout(grid);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    
    Cursors window;
    window.resize(250, 150);
    window.setWindowTitle("Cursors");
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
