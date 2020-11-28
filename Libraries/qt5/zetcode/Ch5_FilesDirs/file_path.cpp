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
        out << "Usage: file_path <file>" << Qt::endl;
        return 1;
    }

    QString filename{argv[1]};
    QFileInfo fi{filename};
    if (!fi.exists()) {
        out << "File not found" << Qt::endl;
        return 1;
    }

    out << "Absolute file path: " << fi.absoluteFilePath() << Qt::endl;
    out << "Base name: " << fi.baseName() << Qt::endl;
    out << "Complete base name: " << fi.completeBaseName() << Qt::endl;
    out << "File name: " << fi.fileName() << Qt::endl;
    out << "Suffix: " << fi.suffix() << Qt::endl;
    out << "Complete suffix: " << fi.completeSuffix() << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
