#include <iostream>
#include <string>
#include <list>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>


namespace fs = boost::filesystem;


int main()
{
    // \u263A is "Unicode WHITE SMILING FACE"
    std::string narrow_string( "smile2" );
    std::wstring wide_string( L"smile2\n263A" );
    std::list<char> narrow_list{'s','m','i','l','e','3'};
    std::list<wchar_t> wide_list{L's',L'm',L'i',L'l',L'e',L'3',L'\u263A'};

    { fs::ofstream f("smile"); }
    { fs::ofstream f(L"smile\u263A"); }
    { fs::ofstream f(narrow_string); }
    { fs::ofstream f(wide_string); }
    { fs::ofstream f(narrow_list); }
    { fs::ofstream f(wide_list); }
    narrow_list.pop_back();
    narrow_list.push_back('4');
    wide_list.pop_back();
    wide_list.pop_back();
    wide_list.push_back(L'4');
    wide_list.push_back(L'u\263A');
    { fs::ofstream f(fs::path(narrow_list.begin(), narrow_list.end())); }
    { fs::ofstream f(fs::path(wide_list.begin(), wide_list.end())); }

    return 0;
}
