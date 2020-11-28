#include <algorithm>
#include <iostream>
#include <QTextStream>
#include <QMap>

int main()
{
try {
    QTextStream out{stdout};

    QMap<QString, int> items{{{"coins", 5}, {"books", 3}}};
    items.insert("bottles", 7);

    QList<int> values{items.values()};
    out << "Values: " << Qt::endl;
    for (int v : values) {
        out << v << Qt::endl;
    }

    QList<QString> keys{items.keys()};
    out << "Keys: " << Qt::endl;
    for (auto const& k : keys) {
        out << k << Qt::endl;
    }

    QMapIterator<QString, int> it{items};
    out << "Pairs: " << Qt::endl;
    while (it.hasNext()) {
        it.next();
        out << it.key() << ": " << it.value() << Qt::endl;
    }
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
