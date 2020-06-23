//
// Created by saad on 6/20/20.
//

#ifndef SFMLPATHFINDER_BFS_H
#define SFMLPATHFINDER_BFS_H

#include "Algorithm.h"
#include <queue>
#include <map>

struct Bfs : public Algorithm {
    std::queue<Rect *> q;                   // queue for search
    std::map<Rect *, bool> discTable;       // for marking if a node has been discovered
    std::map<Rect *, Rect *> parentTable;   // for tracing routes

    bool runComplete(GridPanel &grid) override;
    bool preRun(GridPanel &grid) override;
    bool postRun(GridPanel &grid) override;
    bool stepRun(GridPanel &grid) override;
};

#endif //SFMLPATHFINDER_BFS_H
