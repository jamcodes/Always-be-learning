#include <algorithm>
#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QList<QString> authors{"Balzac", "Tolstoy", "Gulbranssen", "London"};

    for (int i{0}; i != authors.size(); ++i) {
        out << authors.at(i) << Qt::endl;
    }

    authors << "Galsworthy" << "Sienkiewicz";

    out << "************************" << Qt::endl;

    std::sort(authors.begin(), authors.end());

    out << "Sorted: " << Qt::endl;
    for (auto const& author : authors) {
        out << author << Qt::endl;
    }
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
