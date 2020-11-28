#include <iostream>
#include <QTextStream>


int main()
{
try {
    QTextStream out{stdout};

    QString str{"There are many stairs."};

    for (auto ch : str) {
        out << ch << " ";
    }
    out << Qt::endl;

    // iterator interface is supported - begin() and end() members
    // return QString::iterator, which is QChar*
    for (auto it{str.begin()}; it != str.end(); ++it)
    {
        out << *it << " ";
    }
    out << Qt::endl;

    for (int i{0}; i != str.size(); ++i)
    {
        out << str.at(i) << " ";
    }
    out << Qt::endl;

}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
