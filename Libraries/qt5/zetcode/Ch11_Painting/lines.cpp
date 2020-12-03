#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>


class Lines : public QWidget
{
public:
    Lines(QWidget* parent = nullptr)
        : QWidget{parent}
    {
    }

protected:
    void paintEvent(QPaintEvent* /* e */)
    {
        QPainter qp{this};
        drawLines(qp);
    }

    void drawLines(QPainter& p)
    {
        QPen pen{Qt::black, 2, Qt::SolidLine};
        p.setPen(pen);
        p.drawLine(20, 40, 250, 40);

        pen.setStyle(Qt::DashLine);
        p.setPen(pen);
        p.drawLine(20, 80, 250, 80);

        pen.setStyle(Qt::DashDotLine);
        p.setPen(pen);
        p.drawLine(20, 120, 250, 120);

        pen.setStyle(Qt::DotLine);
        p.setPen(pen);
        p.drawLine(20, 160, 250, 160);

        pen.setStyle(Qt::DashDotDotLine);
        p.setPen(pen);
        p.drawLine(20, 200, 250, 200);

        QVector<qreal> dashes;
        const qreal space{4};
        dashes << 1 << space << 5 << space;
        pen.setStyle(Qt::CustomDashLine);
        pen.setDashPattern(dashes);
        p.setPen(pen);
        p.drawLine(20, 240, 250, 240);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    Lines window;
    window.setWindowTitle("Lines");
    window.resize(280, 270);
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
