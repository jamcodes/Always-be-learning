#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;


template<typename Function>
void parallel_dir_iter( fs::path p, Function action );

mutex cout_mutex;

int main( int argc, char* argv[] )
{
    if( argc < 2 ){
        cout << "Usage: parallel_dir_iter path\n";
        return 1;
    }

    auto path = fs::path(argv[1]);
    if( !is_directory(path) && !is_regular_file(path) ){
        cout << "Usage: parallel_dir_iter path\n";
        return 1;
    }

    parallel_dir_iter( path,
        [](const fs::path p)
        { lock_guard<mutex> lk(cout_mutex);
            cout << "thread[" << std::this_thread::get_id()
                << "]: " << p << endl;
        });
}


template<typename Function>
auto parallel_dir_iter( fs::path path, Function action ) -> void
{
    { lock_guard<mutex> lk(cout_mutex);
        cout << "thread[" << std::this_thread::get_id()
            << "] entering directory " << path << endl;
    }
    future<void> res;
    for( const fs::directory_entry& de : fs::directory_iterator(path) ){
        auto p = de.path();
        if( is_directory(p) ){
            res = std::async(parallel_dir_iter<Function>, std::move(p), action);
        }
        else{
            action(p);
        }
    }
    res.wait();
}
