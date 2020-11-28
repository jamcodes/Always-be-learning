#include <algorithm>
#include <iostream>

#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QDateTime>

int main(int argc, char* argv[])
{
try {
    QTextStream out{stdout};

    if (argc != 2) {
        qWarning("Usage: file_times <file>");
        return 1;
    }

    QString filename{argv[1]};
    QFileInfo fileinfo{filename};

    QDateTime last_read{fileinfo.lastRead()};
    QDateTime last_mod{fileinfo.lastModified()};
    out << "Last read: " << last_read.toString() << Qt::endl;
    out << "Last modified: " << last_mod.toString() << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
