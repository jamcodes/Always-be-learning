#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QDate cd{QDate::currentDate()};

    QLocale locale{"en_US"};
    out << "Gregorian date for today: " << locale.toString(cd, "yyyy-MM-dd") << Qt::endl;
    // Julian day is the count of days since the Julian Period - 4713 BC,
    // Julian day 0 is January 1, 4713 BC, at noon.
    // Used by astronomers, military and mainframe systems
    out << "Julian day for today: " << cd.toJulianDay() << Qt::endl;

    QDate bordate{1812, 9, 7};
    QDate slavdate(1805, 12, 2);

    const auto j_today{cd.toJulianDay()};
    const auto j_borodino{bordate.toJulianDay()};
    const auto j_slavkov{slavdate.toJulianDay()};

    out << "Days since Slavkov battle: " << j_today - j_slavkov << Qt::endl;
    out << "Days since Borodino battle: " << j_today - j_borodino << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
