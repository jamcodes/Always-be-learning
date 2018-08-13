#include <iostream>
#include <algorithm>
#include <boost/filesystem.hpp>

using std::cin; using std::cout; using std::endl;
namespace fs = boost::filesystem;


int main( int argc, char* argv[] )
{
    if( argc < 2 ){
        cout << "Usage: tut4 path\n";
        return 1;
    }

    fs::path p(argv[1]);

    try{
        if( exists(p) )
        {
            if( fs::is_regular_file(p) ){
                cout << p << " size is " << file_size(p) << '\n';
            }
            else if(is_directory(p)){
                cout << p << " is a directory containing:\n";
                std::vector<fs::path> v;
                for( auto&& x : fs::directory_iterator(p) )
                    v.push_back(x.path());

                std::sort(v.begin(), v.end());
                for( auto&& x : v )
                    cout << "    " << x.filename() << '\n';
            }
            else
                cout << p << " exists, but is not a regular file or directory\n";
        }
        else
            cout << p << " does not exist\n";
    }
    catch( const fs::filesystem_error& ex ){
        cout << ex.what() << endl;
    }
    return 0;
}
