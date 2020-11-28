#include <iostream>
#include <QTextStream>

int main()
{
try {

    QTextStream out(stdout);

    QString a{"love"};
    a.append(" chess");
    a.prepend("I ");

    out << a << Qt::endl;
    out << "The String has " << a.count() << " characters." << Qt::endl;

    // These operations return copies - non-modifying
    out << a.toUpper() << Qt::endl;
    out << a.toLower() << Qt::endl;

}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
