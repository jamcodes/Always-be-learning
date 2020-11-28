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

    QDir dir;
    if (dir.mkdir("mydir")) {
        out << "mydir created successfully" << Qt::endl;
    }

    dir.mkdir("mydir2");
    if (dir.exists("mydir2")) {
        dir.rename("mydir2", "newdir");
    }
    dir.mkpath("temp/newdir");
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
