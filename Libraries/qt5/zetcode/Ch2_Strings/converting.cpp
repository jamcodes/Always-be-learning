#include <iostream>
#include <QTextStream>

int main()
{
try {
    QTextStream out{stdout};
    QString s1{"12"};
    QString s2{"15"};
    QString hello{"hello"};

    // QStrings can be converted to numeric values
    // toInt(), toFloat() toLong() etc are available
    out << s1.toInt() + s2.toInt() << Qt::endl;

    // if a string can not be converted to a number 0 is returned...
    out << hello.toInt() << Qt::endl;

    // a number can be converted to QString with the .setNum() member
    constexpr int n1{30};
    constexpr int n2{42};
    QString s3, s4;
    out << s3.setNum(n1) + s4.setNum(n2) << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
