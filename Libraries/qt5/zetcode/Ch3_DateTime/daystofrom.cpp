#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QDate dt{2015, 5, 11};
    QDate nd{dt.addDays(55)};

    QDate xmas{2020, 12, 24};

    out << "55 days from " << dt.toString() << " is " << nd.toString() << Qt::endl;
    out << "There are " << QDate::currentDate().daysTo(xmas) << " days to Christmas" << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
