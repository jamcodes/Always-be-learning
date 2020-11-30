#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QKeyEvent>


class KeyPress : public QWidget
{
public:
    KeyPress(QWidget* parent = nullptr)
        : QWidget{parent}
    {
    }

protected:
// In some situations the signal-slot mechanism may not be the best choice,
// an alternative is to reimplement a handler. The QKeyEvent is an event
// object which holds information about what has happened
    void keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_Escape)
        {
            qApp->quit();
        }
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    
    KeyPress window;
    window.resize(250, 150);
    window.setWindowTitle("KeyPress events");
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
