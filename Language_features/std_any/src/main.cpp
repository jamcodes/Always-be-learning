#include <any>
#include <iostream>
#include <string>
#include <vector>

using namespace std::string_literals;

int main()
{
    auto values = std::vector<std::any>();
    values.emplace_back(10);
    auto c_str = "Hello there!";
    values.push_back(c_str);
    values.emplace_back("Wow!"s);
    auto& s = std::any_cast<std::string&>(values.back()); // throws std::bad_any_cast if not string
    s += " That is great!\n";
    std::cout << s;
    auto* ps = std::any_cast<std::string>(&values[1]); // nullptr if not string
    if(!ps){
        std::cout << "That was not a string" << std::endl;
    }
}
