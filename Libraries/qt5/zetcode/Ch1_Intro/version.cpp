#include<iostream>

#include <QtCore>


int main()
{
try {

    std::cout << "Qt version: " << qVersion() << std::endl;

}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
