#include <iostream>
#include <QTextStream>

int main()
{
try {
    QTextStream out{stdout};

    QString a{"Rain"};
    QString b{"rain"};
    QString c{"rain\n"};

    // QString has a ::compare() member - works same as strcmp()
    if (QString::compare(a, b) == 0)
    {
        out << "a, b are equal" << Qt::endl;
    }
    else
    {
        out << "a, b are not equal" << Qt::endl;
    }

    out << "Case-insensitive comparison:\n";
    if (QString::compare(a, b, Qt::CaseInsensitive) == 0)
    {
        out << "a, b are equal" << Qt::endl;
    }
    else
    {
        out << "a, b are not equal" << Qt::endl;
    }

    if (QString::compare(b, c, Qt::CaseInsensitive) == 0)
    {
        out << "b, c are equal" << Qt::endl;
    }
    else
    {
        out << "b, c are not equal" << Qt::endl;
    }

    // mutating member function .chop() - removes characters at the end
    c.chop(1);
    out << "After removing the last character (whitespace):\n";
    if (QString::compare(b, c) == 0)
    {
        out << "b, c are equal" << Qt::endl;
    }
    else
    {
        out << "b, c are not equal" << Qt::endl;
    }
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
