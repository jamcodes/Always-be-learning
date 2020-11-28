#include <iostream>
#include <QTextStream>
#include <QFile>

int main()
{
try {
    QTextStream out{stdout};

    // QString features a member function which coverts plain-text string
    // to an HTML string with HTML metachars <, >, & and " replaced by
    // HTML named entities

    // read an example string
    QFile file{"hello.c"};

    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning("Cannot open file for reading");
        return 1;
    }

    QTextStream in{&file};
    QString allText{in.readAll()};
    // convert to a string with all HTML metachars escaped:
    out << allText.toHtmlEscaped() << Qt::endl;

    file.close();
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
