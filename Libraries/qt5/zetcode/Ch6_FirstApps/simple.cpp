#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>

int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    QWidget window;
    window.resize(250, 150);
    window.setWindowTitle("Simple Example");
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
