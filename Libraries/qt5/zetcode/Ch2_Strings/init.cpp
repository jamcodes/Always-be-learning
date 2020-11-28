#include <iostream>
#include <string>
#include <QTextStream>



int main()
{
try {
    QTextStream out{stdout};
    // initialization of QString

    // From const char*
    QString str1{"The night train"};
    out << str1 << Qt::endl;

    // From std::string
    std::string s1{"A blue sky"};
    QString str2{s1.c_str()};
    out << str2 << Qt::endl;

    std::string s2{"A thick fog"};
    QString str3{QString::fromLatin1(s2.data(), s2.size())};

    const char s3[]{"A deep forest"};
    QString str5{s3};
    out << str5 << Qt::endl;
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
