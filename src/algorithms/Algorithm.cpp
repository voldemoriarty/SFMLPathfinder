//
// Created by saad on 6/22/20.
//

#include "Algorithm.h"

bool Algorithm::checkGrid(GridPanel &grid) {
    grid.clearAllPaths();
    beg = grid.startPoint;
    end = grid.endPoint;

    return !(beg == nullptr || end == nullptr);
}

bool Algorithm::runComplete(GridPanel &grid) {
    if (!preRun(grid))
        return false;

    while (!stepRun(grid));
    return postRun(grid);
}

Algorithm::~Algorithm() = default;

