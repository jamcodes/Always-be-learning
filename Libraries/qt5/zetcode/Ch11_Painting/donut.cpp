#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>


class Donut : public QWidget
{
public:
    Donut(QWidget* parent = nullptr)
        : QWidget{parent}
    {
    }

protected:
    void paintEvent(QPaintEvent* /* e */)
    {
        doPaint();
    }

private:
    void doPaint()
    {
        QPainter p{this};
        p.setPen(QPen{QBrush{"#535353"}, 0.5});
        p.setRenderHint(QPainter::Antialiasing);

        // Move the beginning of the coordinate system into the middle of the screen
        const int h{height()};
        const int w{width()};
        p.translate(QPoint{w/2, h/2});

        for (qreal rot{0}; rot < 360.0; rot += 5.0) {
            p.drawEllipse(-125, -40, 250, 80);
            p.rotate(5.0);
        }
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Donut window;
    window.setWindowTitle("Donut");
    window.resize(350, 280);
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
