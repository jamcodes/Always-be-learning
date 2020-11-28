#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QDate cd{QDate::currentDate()};
    const int wd{cd.dayOfWeek()};

// Deprecated
    // out << "Today is " << QDate::shortDayName(wd) << Qt::endl;
    // out << "Today is " << QDate::longDayName(wd) << Qt::endl;

    QLocale locale{};
    out << "Today is " << locale.dayName(wd, QLocale::FormatType::ShortFormat) << Qt::endl;
    out << "Today is " << locale.dayName(wd, QLocale::FormatType::NarrowFormat) << Qt::endl;
    out << "Today is " << locale.dayName(wd, QLocale::FormatType::LongFormat) << Qt::endl;
    out << "Today is " << locale.standaloneDayName(wd, QLocale::FormatType::ShortFormat) << Qt::endl;
    out << "Today is " << locale.standaloneDayName(wd, QLocale::FormatType::NarrowFormat) << Qt::endl;
    out << "Today is " << locale.standaloneDayName(wd, QLocale::FormatType::LongFormat) << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
