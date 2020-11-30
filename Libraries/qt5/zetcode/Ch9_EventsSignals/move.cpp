#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMoveEvent>


class Move : public QWidget
{

    Q_OBJECT;

public:
    Move(QWidget* parent = nullptr);

protected:
    void moveEvent(QMoveEvent* e);
};

#include "move.moc"

Move::Move(QWidget* parent)
    : QWidget{parent}
{
}

void Move::moveEvent(QMoveEvent* e)
{
    auto const x{e->pos().x()};
    auto const y{e->pos().y()};

    // auto text{QString::number(x) + "," + QString::number(y)};
    auto text{QString{"%1,%2"}.arg(x).arg(y)};

    setWindowTitle(text);
}


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Move window;
    window.resize(250, 150);
    window.setWindowTitle("Move");
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
