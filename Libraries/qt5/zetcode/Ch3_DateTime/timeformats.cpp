#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QTime ct{QTime::currentTime()};

// Deprecated time formatting
//    out << "The time is " << ct.toString(Qt::TextDate) << Qt::endl;
//    out << "The time is " << ct.toString(Qt::ISODate) << Qt::endl;
//    out << "The time is " << ct.toString(Qt::SystemLocaleShortDate) << Qt::endl;
//    out << "The time is " << ct.toString(Qt::SystemLocaleLongDate) << Qt::endl;
//    out << "The time is " << ct.toString(Qt::DefaultLocaleShortDate) << Qt::endl;
//    out << "The time is " << ct.toString(Qt::DefaultLocaleLongDate) << Qt::endl;
//    out << "The time is " << ct.toString(Qt::SystemLocaleDate) << Qt::endl;
//    out << "The time is " << ct.toString(Qt::LocaleDate) << Qt::endl;
    QLocale locale{};
    out << "The time is " << locale.toString(ct, locale.timeFormat(QLocale::FormatType::ShortFormat)) << Qt::endl;
    out << "The time is " << locale.toString(ct, locale.timeFormat(QLocale::FormatType::NarrowFormat)) << Qt::endl;
    out << "The time is " << locale.toString(ct, locale.timeFormat(QLocale::FormatType::LongFormat)) << Qt::endl;
    out << "The time is " << locale.toString(ct, "hh:mm:ss.zzz") << Qt::endl;
    out << "The time is " << locale.toString(ct, "h:m:s a") << Qt::endl;
    out << "The time is " << locale.toString(ct, "H:m:s A") << Qt::endl;
    out << "The time is " << locale.toString(ct, "h:m AP") << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
