#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QDate cd{QDate::currentDate()};

// Deprecated members
    // out << "Today is " << cd.toString(Qt::DateFormat::TextDate) << Qt::endl;
    // out << "Today is " << cd.toString(Qt::DateFormat::ISODate) << Qt::endl;
    // out << "Today is " << cd.toString(Qt::DateFormat::SystemLocaleShortDate) << Qt::endl;
    // out << "Today is " << cd.toString(Qt::DateFormat::SystemLocaleLongDate) << Qt::endl;
    // out << "Today is " << cd.toString(Qt::DateFormat::SystemLocaleDate) << Qt::endl;
    // out << "Today is " << cd.toString(Qt::DateFormat::DefaultLocaleShortDate) << Qt::endl;
    // out << "Today is " << cd.toString(Qt::DateFormat::DefaultLocaleLongDate) << Qt::endl;
    // out << "Today is " << cd.toString(Qt::DateFormat::LocalDate) << Qt::endl;
    QLocale locale{};
    out << "Today is " << locale.toString(cd, locale.dateFormat(QLocale::FormatType::ShortFormat)) << Qt::endl;
    out << "Today is " << locale.toString(cd, locale.dateFormat(QLocale::FormatType::NarrowFormat)) << Qt::endl;
    out << "Today is " << locale.toString(cd, locale.dateFormat(QLocale::FormatType::LongFormat)) << Qt::endl;
    out << "Today is " << locale.toString(cd, "yyyy-MM-dd") << Qt::endl;
    out << "Today is " << locale.toString(cd, "yy/M/dd") << Qt::endl;
    out << "Today is " << locale.toString(cd, "d. M. yyyy") << Qt::endl;
    out << "Today is " << locale.toString(cd, "d-MMMM-yyyy") << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
