#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

#include "breakout.h"


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    Breakout window;
    window.resize(300, 400);
    window.setWindowTitle("Breakout");
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
