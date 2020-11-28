#include <iostream>
#include <QTextStream>


int main()
{
try {

    QTextStream out{stdout};

    QString a{"Eagle"};
    // QString is a sequence of QChars
    out << a[0] << Qt::endl;
    out << a[4] << Qt::endl;
    out << a.at(0) << Qt::endl;

    // QString at() returns a proxy that can be a nullptr if the index is out-of-range
    // in debug-builds accessing out-of-range will trigger an assertion
    if (a.at(5).isNull()) {
        out << "Outside the range of the string" << Qt::endl;
    }
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
