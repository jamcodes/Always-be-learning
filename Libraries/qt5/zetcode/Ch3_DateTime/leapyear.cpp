#include <algorithm>
#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>


int main()
{
try {
    QTextStream out{stdout};

    QList<int> years;
    std::generate_n(std::back_inserter(years), 5, [i=2011]()mutable{ return i++; });

    for (auto year : years)
    {
        if (QDate::isLeapYear(year)) {
            out << year << " is a leap year\n";
        }
        else
        {
            out << year << " is not a leap year\n";
        }
    }
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
