//
// Created by saad on 6/30/20.
//

#include "Dfs.h"

//
// Created by saad on 6/20/20.
//

bool Dfs::preRun(GridPanel &grid) {
    if (!checkGrid(grid)) {
        return false;
    }

    // mark root as discovered
    discTable.insert(beg);
    stack.push(beg);
    return true;
}

bool Dfs::postRun(GridPanel &grid) {
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

bool Dfs::stepRun(GridPanel &grid) {
    if (stack.empty())
        return true;

    auto v = stack.top();
    stack.pop();

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
                discTable.insert(neighbour);
                parentTable.insert_or_assign(neighbour, v);
                stack.push(neighbour);
            }
        }
    }

    return false;
}

void Dfs::reset() {
    discTable.clear();
    parentTable.clear();
    stack = std::stack<Rect *>{};
}
