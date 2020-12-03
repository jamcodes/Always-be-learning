#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>


class RadialGradient : public QWidget
{
public:
    RadialGradient(QWidget* parent = nullptr)
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

        const auto h{static_cast<qreal>(height())};
        const auto w{static_cast<qreal>(width())};

        QRadialGradient grad1{w/2, h/2, 80};
        grad1.setColorAt(0, QColor{"#032E91"});
        grad1.setColorAt(0.3, Qt::white);
        grad1.setColorAt(1, QColor{"#032E91"});

        p.fillRect(0, 0, w, h, grad1);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    RadialGradient window;
    window.resize(300, 250);
    window.setWindowTitle("Radial Gradient");
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
