#include <algorithm>
#include <iostream>

#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QDir>

int main()
{
try {
    QTextStream out{stdout};

    out << "Current path: " << QDir::currentPath() << Qt::endl;
    out << "Home path: " << QDir::homePath() << Qt::endl;
    out << "Temporary path: " << QDir::tempPath() << Qt::endl;
    out << "Root path: " << QDir::rootPath() << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
