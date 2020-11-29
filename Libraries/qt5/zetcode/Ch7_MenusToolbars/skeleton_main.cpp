#include <iostream>

#include "skeleton.h"

int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Skeleton window;
    window.resize(350, 250);
    window.setWindowTitle("Application Skeleton");
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
