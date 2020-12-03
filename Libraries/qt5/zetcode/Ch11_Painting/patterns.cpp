#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>


class Patterns : public QWidget
{
public:
    Patterns(QWidget* parent = nullptr)
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
        p.setPen(Qt::NoPen);

        p.setBrush(Qt::HorPattern);
        p.drawRect(10, 15, 90, 60);

        p.setBrush(Qt::VerPattern);
        p.drawRect(130, 15, 90, 60);

        p.setBrush(Qt::CrossPattern);
        p.drawRect(250, 15, 90, 60);

        p.setBrush(Qt::Dense7Pattern);
        p.drawRect(10, 105, 90, 60);

        p.setBrush(Qt::Dense6Pattern);
        p.drawRect(130, 105, 90, 60);

        p.setBrush(Qt::Dense5Pattern);
        p.drawRect(250, 105, 90, 60);

        p.setBrush(Qt::BDiagPattern);
        p.drawRect(10, 195, 90, 60);

        p.setBrush(Qt::FDiagPattern);
        p.drawRect(130, 195, 90, 60);

        p.setBrush(Qt::DiagCrossPattern);
        p.drawRect(250, 195, 90, 60);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Patterns window;
    window.setWindowTitle("Patterns");
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
