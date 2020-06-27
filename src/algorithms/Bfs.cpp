//
// Created by saad on 6/20/20.
//

#include "Bfs.h"

bool Bfs::preRun(GridPanel &grid) {
    tiles = 0;
    if (!checkGrid(grid)) {
        return false;
    }

    // mark root as discovered
    discTable[beg] = true;
    q.push(beg);
    return true;
}

bool Bfs::postRun(GridPanel &grid) {
    // at this point, we have either found the end
    // in which case, its parent is available in parent table
    // else we couldn't find it

    // draw the path; if exists
    auto parentItr = parentTable.find(end);
    bool found = parentItr != parentTable.end();

    if (found) {
        // path exists
        auto rect = parentItr->second;
        while (rect != beg) {
            grid.changeRect(*rect, RectType::path);
            rect = parentTable[rect];
        }
    }

    reset();
    return found;
}

bool Bfs::stepRun(GridPanel &grid) {
    tiles++;
    if (q.empty())
        return true;

    auto v = q.front();
    q.pop();

    if (v == end) {
        grid.clearFocus();
        return true;
    }

    // highlight the rect
    grid.putInFocus(v);

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

    return false;
}

void Bfs::reset() {
    discTable.clear();
    parentTable.clear();
    q = std::queue<Rect *>{};
}
