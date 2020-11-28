#include <algorithm>
#include <iostream>

#include <QTextStream>
#include <QFileInfo>

int main(int argc, char* argv[])
{
try {
    QTextStream out{stdout};

    if (argc != 2) {
        qWarning("Usage: file_size <file>");
        return 1;
    }

    QString filename{argv[1]};

    if (!QFile{filename}.exists()) {
        qWarning("The file does not exist");
        return 1;
    }

    QFileInfo fileinfo{filename};
    qint64 size{fileinfo.size()};
    QString str{"The size is: %1 bytes"};
    out << str.arg(size) << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
