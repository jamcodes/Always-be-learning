#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QVector<int> vals{1, 2, 3, 4, 5};
    out << "The size of the vector is: " << vals.size() << Qt::endl;
    out << "The first item is: " << vals.first() << Qt::endl;
    out << "The last item is: " << vals.last() << Qt::endl;

    vals.append(6);
    vals.prepend(0);

    out << "Elements: ";
    for (auto v : vals) {
        out << v << " ";
    }

    out << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
