#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <ctime>

int main()
{
try {
    QTextStream out{stdout};

    time_t t{time(0)};
    out << "time_t{0} = " << t << Qt::endl;

    QDateTime dt;
    dt.setTime_t(t);
    out << ".setTime_t(t) = " << dt.toString() << Qt::endl;

    QDateTime cd{QDateTime::currentDateTime()};
    out << "currentDateTime().toTime_t = " << cd.toTime_t() << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
