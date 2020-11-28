#include <algorithm>
#include <iostream>

#include <QTextStream>
#include <QFileInfo>

int main()
{
try {
    QTextStream out{stdout};

    QFile file{"colors"};
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Cannot open file for reading");
        return 1;
    }

    QTextStream in{&file};
    while (!in.atEnd()) {
        QString line{in.readLine()};
        out << line << Qt::endl;
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
