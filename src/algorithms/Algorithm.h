//
// Created by saad on 6/20/20.
//

#ifndef SFMLPATHFINDER_ALGORITHM_H
#define SFMLPATHFINDER_ALGORITHM_H

#include "../GridPanel.h"

struct Algorithm {
    using Rect = GridPanel::Rect;

    Rect* beg = nullptr;
    Rect* end = nullptr;

    bool checkGrid(GridPanel &grid);

    virtual void run(GridPanel &grid) = 0;
};

#endif //SFMLPATHFINDER_ALGORITHM_H
