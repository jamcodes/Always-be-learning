#include <algorithm>
#include <iostream>
#include <QTextStream>

int main()
{
try {
    QTextStream out{stdout};

    QString str{"coin, book, cup, pencil, clock, bookmark"};
    // QStringList is a convenience type for operating on a list of strings
    QStringList items{str.split(",")};
    QStringListIterator it{items};

    while (it.hasNext()) {
        out << it.next().trimmed() << Qt::endl;
    }

}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
