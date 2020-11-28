#include <iostream>
#include <QTextStream>

int main()
{
try {
    QTextStream out{stdout};

    QString field1{"Name: "};
    QString field2{"Occupation: "};
    QString field3{"Residence: "};
    QString field4{"Marital status: "};

    const int width{field4.size()};

    // QString can be justified - to left/right etc
    out << field1.rightJustified(width, ' ') << "Robert\n";
    out << field2.rightJustified(width, ' ') << "programmer\n";
    out << field3.rightJustified(width, ' ') << "New York\n";
    out << field4.rightJustified(width, ' ') << "single\n";
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
