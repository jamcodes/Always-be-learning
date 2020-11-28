#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QList<QDate> dates{{
        QDate{2015, 5, 11},
        QDate{2015, 8, 1},
        QDate{2015, 2, 30}
    }};

    QLocale locale{"en_US"};
    for (int i{0}; i != dates.size(); ++i)
    {
        if (dates.at(i).isValid())
        {
            out << "Date " << i+1 << " is a valid date: " << locale.toString(dates.at(i)) << Qt::endl;
        }
        else
        {
            out << "Date " << i+1 << " is not a valid date" << Qt::endl;
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
