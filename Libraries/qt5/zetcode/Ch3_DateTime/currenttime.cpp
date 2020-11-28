#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QDate cd{QDate::currentDate()};
    QTime ct{QTime::currentTime()};

    out << "Current date is " << cd.toString() << Qt::endl;
    out << "Current time is " << ct.toString() << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
