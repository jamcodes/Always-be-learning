#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QDate dt1{2015, 9, 18};
    QDate dt2{2015, 2, 11};
    QDate dt3{2015, 5, 1};
    QDate dt4{2015, 12, 11};
    QDate dt5{2015, 1, 21};

    QLocale locale{"en_US"};
    out << "There are " << dt1.daysInMonth() << " days in " << locale.monthName(dt1.month()) << Qt::endl;
    out << "There are " << dt2.daysInMonth() << " days in " << locale.monthName(dt2.month()) << Qt::endl;
    out << "There are " << dt3.daysInMonth() << " days in " << locale.monthName(dt3.month()) << Qt::endl;
    out << "There are " << dt4.daysInMonth() << " days in " << locale.monthName(dt4.month()) << Qt::endl;
    out << "There are " << dt5.daysInMonth() << " days in " << locale.monthName(dt5.month()) << Qt::endl;
    out << "There are " << dt1.daysInYear() << " days in year " << QString::number(dt1.year()) << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
