#include <iostream>
#include <boost/filesystem.hpp>

using std::cout; using std::endl;
namespace fs = boost::filesystem;


int main( int argc, char* argv[] )
{
    if( argc < 2 ){
        std::cout << "Usage: tut3 path\n";
        return 1;
    }

    fs::path p(argv[1]);
    try{
        if( fs::exists(p) )
        {
        if( fs::is_regular_file(p) )
            cout << p << " size is " << fs::file_size(p) << "\n";
        else if( fs::is_directory(p) ){
            cout << p << " is a directory containing:\n";
            for( fs::directory_entry& x : fs::directory_iterator(p) ){
                cout << "    " << x.path() << "\n";
            }
        }
        else
            cout << p << " does not exist\n";
        }
    }
    catch( const fs::filesystem_error& ex ){
        std::cerr << ex.what() << '\n';
    }

    return 0;
}
