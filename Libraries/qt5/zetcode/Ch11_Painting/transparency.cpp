#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QPainterPath>


class TransparentRectangles : public QWidget
{
public:
    TransparentRectangles(QWidget* parent = nullptr)
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

        for (int i{1}; i <= 11; ++i)
        {
            p.setOpacity(i * 0.1);
            p.fillRect(50*i, 20, 40, 40, Qt::darkGray);
        }
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    TransparentRectangles window;
    window.setWindowTitle("Transparent Rectangles");
    window.resize(630, 90);
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
