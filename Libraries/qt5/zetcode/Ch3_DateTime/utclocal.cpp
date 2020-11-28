#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QDateTime cdt{QDateTime::currentDateTime()};

    out << "Universal datetime: " << cdt.toUTC().toString() << Qt::endl;
    out << "Local datetime: " << cdt.toLocalTime().toString() << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
