#include <iostream>
#include <QTextStream>


int main()
{
try {
    QTextStream out{stdout};
    QString str{"The night train"};

    // substrings can be created with .left() . mid() and .right() member functions
    // there return copies
    out << str.right(5) << Qt::endl;
    out << str.left(9) << Qt::endl;
    out << str.mid(4, 5) << Qt::endl;

    QString str2{"The big apple"};
    // string_view-like object - string with reference semantics
    QStringRef sub{&str2, 0, 7};

    out << sub << Qt::endl;
    out << sub.toString() << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
