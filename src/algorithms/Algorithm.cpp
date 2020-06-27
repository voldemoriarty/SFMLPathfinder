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
    if (!init(grid))
        return false;

    while (!step(grid));
    return postRun(grid);
}

bool Algorithm::step(GridPanel &grid) {
    tiles++;
    return stepRun(grid);
}

bool Algorithm::init(GridPanel &grid) {
    tiles = 0;
    return preRun(grid);
}

Algorithm::~Algorithm() = default;

