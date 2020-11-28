#include <iostream>
#include <QTextStream>

int main()
{
try {

    QTextStream out{stdout};

    QString s1{"Eagle"};
    QString s2{"Eagle\n"};
    QString s3{"Eagle "};
    QString s4{"opęn"};

    // QString has .size(), .count() .length()
    // all of which return the number of characters
    out << "s1.length() = " << s1.length() << Qt::endl;
    out << "s1.size() = " << s1.size() << Qt::endl;
    out << "s1.count() = " << s1.count() << Qt::endl;
    out << s2.size() << Qt::endl;
    out << s3.count() << Qt::endl;

}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
