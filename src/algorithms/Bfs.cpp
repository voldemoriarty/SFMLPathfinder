//
// Created by saad on 6/20/20.
//

#include "Bfs.h"

void Bfs::run(GridPanel &grid) {
    discTable.clear();
    parentTable.clear();

    auto start      = grid.startPoint;
    auto end        = grid.endPoint;

    // do nothing if start or end is undefined
    if (start == nullptr || end == nullptr) {
        return;
    }

    // mark root as discovered
    discTable[start] = true;
    q.push(start);

    while (!q.empty()) {
        auto v = q.front();
        q.pop();

        if (v == end) {
            return;
        }

        // iterate all the neighbours
        // start from north then go clockwise
        for (int i = 0; i < 4; ++i) {
            auto neighbour = grid.findNeighbour(v, i);

            // if the neighbour exists and is not a wall
            if (neighbour != nullptr && neighbour->second != RectType::wall) {
                // check if the neighbour isn't already discovered
                if (discTable.count(neighbour) == 0) {
                    discTable.insert_or_assign(neighbour, true);
                    parentTable.insert_or_assign(neighbour, v);
                    q.push(neighbour);
                }
            }
        }
    }

    // at this point, we have either found the end
    // in which case, its parent is available in parent table
    // else we couldn't find it

    // draw the path; if exists
    auto parentItr = parentTable.find(end);

    if (parentItr != parentTable.end()) {
        // path exists
        auto rect = parentItr->second;
        while (rect != start) {
            grid.toggleRect(*rect, RectType::path);
            rect = parentTable[rect];
        }
    }
}
