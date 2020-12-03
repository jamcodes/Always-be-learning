#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>


class LinearGradients : public QWidget
{
public:
    LinearGradients(QWidget* parent = nullptr)
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

        QLinearGradient grad1{0, 20, 0, 110};
        grad1.setColorAt(0.1, Qt::black);
        grad1.setColorAt(0.5, Qt::yellow);
        grad1.setColorAt(0.9, Qt::black);

        p.fillRect(20, 20, 300, 90, grad1);

        QLinearGradient grad2{0, 55, 250, 0};
        grad2.setColorAt(0.2, Qt::black);
        grad2.setColorAt(0.5, Qt::red);
        grad2.setColorAt(0.8, Qt::black);

        p.fillRect(20, 140, 300, 100, grad2);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    LinearGradients window;
    window.setWindowTitle("Linear Gradients");
    window.resize(350, 260);
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
