#pragma once

#include "PunyPyWorld.h"
#include "Production.h"

class Interpreter
{
public:
    Interpreter(PunyPyWorld& world, std::vector<Production>& productions)
        : world_{world}, productions_{productions} { }

    void interpret()
    {
        for(auto& p : productions_){
            p.interpret(world_);
        }
    }
private:
    PunyPyWorld& world_;
    std::vector<Production>& productions_;
};
