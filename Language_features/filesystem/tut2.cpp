#include <iostream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
using std::cin; using std::cout;
using std::endl;


int main( int argc, char* argv[] )
{
    if( argc < 2 ){
        cout << "Usage: tut2 path\n";
        return 1;
    }

    fs::path p( argv[1] );
    if( fs::exists(p) ){
        if( fs::is_regular_file(p) ){
            cout << p <<" size is " << file_size(p) << '\n';
        }
        else if( fs::is_directory(p) )
            cout << p << " is a directory\n";
        else
            cout << p << " eists, but is not a regular file or directory\n";
    }
    else
        cout << p << " does not exists\n";

    return 0;
}
