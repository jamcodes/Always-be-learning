#include <algorithm>
#include <iostream>

#include <QTextStream>
#include <QApplication>
#include <QWidget>
#include <QIcon>

int main(int argc, char* argv[])
{
try {
    QTextStream out{stdout};
    QApplication app{argc, argv};
    QWidget window;
    window.resize(250, 150);
    window.setWindowTitle("Icon");
    const auto icon_path{QCoreApplication::applicationDirPath() + "/kiwi.png"};
    out << "app_path: " << icon_path << Qt::endl;

    window.setWindowIcon(QIcon{icon_path});
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
