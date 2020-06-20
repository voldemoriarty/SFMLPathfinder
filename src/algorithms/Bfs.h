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
    void run(GridPanel &grid) override;
};

#endif //SFMLPATHFINDER_BFS_H
