#include <iostream>
#include "Parser.h"
#include "PunyPyWorld.h"
#include "Analyzer.h"
#include "Interpreter.h"


int main()
{
    auto parser = Parser{std::cin};
    auto prods = std::vector<Production>();
    for(auto p = parser.root(); /**/; p = parser.root())
    try{
            std::cout << p.rep() << std::endl;
            prods.push_back(std::move(p));
            if(!parser)
                break;
    }
    catch(std::exception& err){
        std::cerr << "\nParsing error: " << err.what();
    }

    auto world = PunyPyWorld{};
    world.set_buildin("print",
        [](const std::vector<int>& params){
            for(auto p : params){
                std::cout << p << ", ";
            }
            return 0;
        }
    );
    auto analyzer = Analyzer{world, prods};

    try{
        analyzer.analyze();
    }
    catch(std::exception& err){
        std::cerr << "\nAnalysis error: " << err.what();
    }
    try{

        auto interpreter = Interpreter{world, prods};
        interpreter.interpret();
    }
    catch(std::exception& err){
        std::cerr << "\nInterpreter error: " << err.what();
    }

    return 0;
}
