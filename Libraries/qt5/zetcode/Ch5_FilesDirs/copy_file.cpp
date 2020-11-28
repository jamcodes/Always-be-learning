#include <algorithm>
#include <iostream>

#include <QTextStream>
#include <QFileInfo>
#include <QFile>

int main(int argc, char* argv[])
{
try {
    QTextStream out{stdout};

    if (argc != 3) {
        qWarning("Usage: copy_file <source> <destination>");
        return 1;
    }

    QString source{argv[1]};

    if (!QFile{source}.exists()) {
        qWarning("The source file does not exist");
        return 1;
    }

    QString dest{argv[2]};

    QFile::copy(source, dest);
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
