#include <iostream>
#include <QTextStream>
#include <QDateTime>

int main()
{
try {
    QTextStream out{stdout};

    // QDateTime is a combination of QDate and QTime
    // it exposes similar methods to the other two and the usage is identical
    QDateTime cdt{QDateTime::currentDateTime()};

    out << "The current datetime is " << cdt.toString() << Qt::endl;
    out << "The current date is " << cdt.date().toString() << Qt::endl;
    out << "The current time is " << cdt.time().toString() << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
