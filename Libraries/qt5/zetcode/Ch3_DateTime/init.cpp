#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QDate dt1{2015, 4, 12};
    out << "The date is " << dt1.toString() << Qt::endl;

    dt1.setDate(2015, 3, 3);
    out << "The date is " << dt1.toString() << Qt::endl;

    QTime tm1{17, 30, 12, 55};
    out << "The time is " << tm1.toString() << Qt::endl;

    tm1.setHMS(13, 52, 45, 155);
    out << "The time is " << tm1.toString() << Qt::endl;

}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
