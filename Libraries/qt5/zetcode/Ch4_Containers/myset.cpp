#include <algorithm>
#include <iostream>
#include <QTextStream>
#include <QSet>
#include <QList>

int main()
{
try {
    QTextStream out{stdout};

    QSet<QString> cols1{"yellow", "red", "blue"};
    QSet<QString> cols2{"blue", "pink", "orange"};

    out << "There are " << cols1.size() << " values in the set" << Qt::endl;
    cols1.insert("brown");
    out << "There are " << cols1.size() << " values in the set" << Qt::endl;

    // union
    cols1.unite(cols2);
    out << "There are " << cols1.size() << " values in the set" << Qt::endl;

    for (auto const& v : cols1) {
        out << v << Qt::endl;
    }

    QList<QString> lcols{cols1.values()};
    std::sort(lcols.begin(), lcols.end());

    out << "*****************************" << Qt::endl;
    out << "Sorted: " << Qt::endl;
    for (auto const& v : lcols) {
        out << v << Qt::endl;
    }

}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
