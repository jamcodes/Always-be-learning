#include <iostream>
#include <QTextStream>
#include <QDate>
#include <QTime>

int main()
{
try {
    QTextStream out{stdout};

    QDate d1{2015, 4, 5};
    QDate d2{2014, 4, 5};


    auto cmp = [&out](QDate const& lhs, QDate const& rhs)
    {
        if (lhs == rhs)
        {
            out << lhs.toString() << " is the same date as " << rhs.toString() << Qt::endl;
        }
        else if (lhs < rhs)
        {
            out << lhs.toString() << " comes before " << rhs.toString() << Qt::endl;
        }
        else
        {
            out << lhs.toString() << " comes after " << rhs.toString() << Qt::endl;
        }
    };
    cmp(d1, d2);


    d2.setDate(d1.year(), d1.month(), d1.day());
    cmp(d1, d2);
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
