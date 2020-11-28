#include <algorithm>
#include <iostream>

#include <QTextStream>
#include <QFileInfo>
#include <QFile>

int main(int argc, char* argv[])
{
try {
    QTextStream out{stdout};

    if (argc != 2) {
        qWarning("Usage: owner <file>");
        return 1;
    }

    QString filename{argv[1]};
    QFileInfo fileinfo{filename};
    if (!fileinfo.exists()) {
        qWarning("File not found");
        return 1;
    }

    QString group{fileinfo.group()};
    QString owner{fileinfo.owner()};

    out << "Group: " << group << Qt::endl;
    out << "Owner: " << owner << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
