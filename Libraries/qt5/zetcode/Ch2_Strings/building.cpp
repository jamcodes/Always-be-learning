#include <iostream>
#include <QTextStream>


int main()
{
try {
    QTextStream out{stdout};

    // QString has a dynamic string-building feature
    // specific control-characters can be replaced with actual values
    QString s1{"There are %1 white roses"};
    const int n{12};
    out << s1.arg(n) << Qt::endl;

    QString s2{"The tree is %1 m high"};
    const double h{5.56};
    out << s2.arg(h) << Qt::endl;

    QString s3{"We have %1 lemons and %2 oranges"};
    constexpr int ln{12};
    constexpr int on{4};
    out << s3.arg(ln).arg(on) << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
