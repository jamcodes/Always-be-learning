#include <iostream>
#include <QTextStream>


int main()
{
try {
    QTextStream out{stdout};

    int digits{0};
    int letters{0};
    int spaces{0};
    int puncts{0};

    QString str{"7 white, 4 red roses.\n"};

    for (QChar ch : str)
    {
        if (ch.isDigit()) { ++digits; }
        else if (ch.isLetter()) { ++letters; }
        else if (ch.isSpace()) { ++spaces; }
        else if (ch.isPunct()) { ++puncts; }
    }

    out << QString{"There are %1 characters\n"}.arg(str.size());
    out << QString{"There are %1 letters\n"}.arg(letters);
    out << QString{"There are %1 digits\n"}.arg(digits);
    out << QString{"There are %1 whitespace characters\n"}.arg(spaces);
    out << QString{"There are %1 punctuation characters\n"}.arg(puncts);
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
