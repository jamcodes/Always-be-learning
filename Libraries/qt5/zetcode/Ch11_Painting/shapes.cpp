#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QPainterPath>


class Shapes : public QWidget
{
public:
    Shapes(QWidget* parent = nullptr)
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

        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(QPen{QBrush{"#888"}, 1});
        p.setBrush(QBrush{QColor{"#888"}});

        QPainterPath path1;
        path1.moveTo(5, 5);
        path1.cubicTo(40, 5, 50, 50, 99, 99);
        path1.cubicTo(5, 99, 50, 50, 5, 5);
        p.drawPath(path1);

        p.drawPie(130, 20, 90, 60, 30*16, 120*16);
        p.drawChord(240, 30, 90, 60, 0, 16*180);
        // p.drawRoundedRect(20, 120, 80, 50);
        p.drawRoundedRect(QRectF{20, 120, 80, 50}, qreal{80.0}, qreal{50.0});

        QPolygon polygon({QPoint{130, 140}, QPoint{180, 170}, QPoint{180, 140}, QPoint{220, 110}, QPoint{140, 100}});

        p.drawPolygon(polygon);
        p.drawRect(250, 110, 60, 60);

        QPointF baseline{20, 250};
        QFont font{"Georgia", 55};
        QPainterPath path2;
        path2.addText(baseline, font, "Q");
        p.drawPath(path2);

        p.drawEllipse(140, 200, 60, 60);
        p.drawEllipse(240, 200, 90, 60);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    Shapes window;
    window.setWindowTitle("Shapes");
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
