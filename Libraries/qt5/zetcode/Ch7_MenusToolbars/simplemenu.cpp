#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>


// Menus are only part of the MainWindow
class SimpleMenu : public QMainWindow {
public:
    SimpleMenu(QWidget* parent = nullptr)
        : QMainWindow{parent}
    {
        auto* action_quit{new QAction{"&Quit", this}};

        QMenu* menu_file{menuBar()->addMenu("&File")};
        // QMenus have operate on "action" objects
        menu_file->addAction(action_quit);
        connect(action_quit, &QAction::triggered, qApp, QApplication::quit);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    SimpleMenu window;
    window.resize(250, 150);
    window.setWindowTitle("Simple Menu");
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
