#include <algorithm>
#include <iostream>

#include <QTextStream>
#include <QFileInfo>
#include <QFile>

int main()
{
try {
    QTextStream out{stdout};

    QString filename{"distros"};
    QFile file{filename};

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream fout{&file};
        fout << "Xubuntu" << Qt::endl;
        fout << "Arch" << Qt::endl;
        fout << "Debian" << Qt::endl;
        fout << "RedHat" << Qt::endl;
        fout << "Slackware" << Qt::endl;
    }
    else
    {
        qWarning("Could not open file for writing");
    }

    file.close();
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
