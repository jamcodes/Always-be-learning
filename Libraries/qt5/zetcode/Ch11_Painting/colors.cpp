#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>


class Colors : public QWidget
{
public:
    Colors(QWidget* parent = nullptr)
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
        p.setPen(QColor{"#d4d4d4"});

        p.setBrush(QBrush{"#c56c00"});
        p.drawRect(10, 15, 90, 60);

        p.setBrush(QBrush{"#lac500"});
        p.drawRect(130, 15, 90, 60);

        p.setBrush(QBrush{"#539e47"});
        p.drawRect(250, 15, 90, 60);

        p.setBrush(QBrush{"#004fc5"});
        p.drawRect(10, 105, 90, 60);

        p.setBrush(QBrush{"#c50024"});
        p.drawRect(130, 105, 90, 60);

        p.setBrush(QBrush{"#9e4575"});
        p.drawRect(250, 105, 90, 60);

        p.setBrush(QBrush{"#5f3b00"});
        p.drawRect(10, 195, 90, 60);

        p.setBrush(QBrush{"#4c4c4c"});
        p.drawRect(130, 195, 90, 60);

        p.setBrush(QBrush{"#785f36"});
        p.drawRect(250, 195, 90, 60);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Colors window;
    window.setWindowTitle("Colors");
    window.resize(360, 280);
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
