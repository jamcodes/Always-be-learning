#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <utility>
#include <stdexcept>

#include "debug.h"
#include "Production.h"
// class  Production;


struct Bad_declaration : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class PunyPyWorld
{
public:
    using variable_t = std::pair<std::string, int>;
    using iterator = std::unordered_map<std::string, int>::iterator;
    using const_iterator = std::unordered_map<std::string, int>::const_iterator;

    PunyPyWorld() = default;
    explicit PunyPyWorld(const std::vector<variable_t>& vars)
        : variables_{vars.cbegin(), vars.cend()} { }


    PunyPyWorld(const PunyPyWorld&) = default;
    PunyPyWorld& operator=(const PunyPyWorld&) = default;
    PunyPyWorld(PunyPyWorld&&) noexcept = default;
    PunyPyWorld& operator=(PunyPyWorld&&) = default;
    ~PunyPyWorld() noexcept = default;

    // auto begin() noexcept { return variables_.begin(); }
    // auto end() noexcept   { return variables_.end(); }
    // auto begin() const noexcept { return variables_.begin(); }
    // auto end() const noexcept { return variables_.end(); }
    // auto cbegin() const noexcept { return variables_.cbegin(); }
    // auto cend() const noexcept { return variables_.cend(); }

    std::pair<int, bool> get_var(const std::string& name) const noexcept;
    void set_var(const std::string& name, int val);
    Function* get_func(const std::string& name);
    void set_func(const std::string& name, Function&& prod);
    std::function<int(std::vector<int> const&)> get_buildin(const std::string& name);
    template<typename Func>
    void set_buildin(const std::string& name, Func&& f);

private:
    std::unordered_map<std::string, int> variables_{};
    std::unordered_map<std::string, Function> functions_{};
    std::unordered_map<std::string, std::function<int(const std::vector<int>&)>> buildins_{};
};

template<typename Func>
void PunyPyWorld::set_buildin(const std::string& name, Func&& f)
{
    auto res = buildins_.emplace(name, std::forward<Func>(f));
    if(!res.second){
        throw Bad_declaration{"Redeclaration of a buildin function " + name};
    }
}
