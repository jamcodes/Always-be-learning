#include <iostream>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;



int main( int argc, char* argv[] )
{
    if( argc < 2 ){
        std::cout << "Usage: " << argv[0] << " path" << std::endl;
        return 1;
    }

    std::cout << "argv[1] size = " << fs::file_size(argv[1]);
    return 0;
}
