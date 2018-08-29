#pragma once

#include "PunyPyWorld.h"
#include "Production.h"

class Analyzer
{
public:
    explicit Analyzer(PunyPyWorld& world, std::vector<Production>& productions)
        : world_{world}, productions_{productions} { }

    void analyze()
    {
        for(auto& p : productions_){
            p.analyze(world_);
        }
    }

private:
    PunyPyWorld& world_;
    std::vector<Production>& productions_;
};
