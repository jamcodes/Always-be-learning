#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

#include "application.h"


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    Application window;
    window.resize(370, 200);
    window.setWindowTitle("The Burning Widget");
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
