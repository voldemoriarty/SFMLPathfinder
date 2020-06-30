//
// Created by saad on 6/30/20.
//

#pragma once

#include "../Algorithm.h"
#include <stack>
#include <map>
#include <set>

struct Dfs : public Algorithm {
    std::stack<Rect *>          stack;             // queue for search
    std::set<Rect *>            discTable;     // for marking if a node has been discovered
    std::map<Rect *, Rect *>    parentTable;   // for tracing routes

    bool preRun(GridPanel &grid) override;
    bool postRun(GridPanel &grid) override;
    bool stepRun(GridPanel &grid) override;
    void reset() override;
};
