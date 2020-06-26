//
// Created by saad on 6/20/20.
//

#pragma once

#include "../GridPanel.h"

struct Algorithm {
    using Rect = GridPanel::Rect;

    // the start and end points in the grid
    Rect* beg = nullptr;
    Rect* end = nullptr;

    // check if the grid is ok to run the search
    // checks if beg and end are not nullptr
    // common to all implementations hence implemented in
    // base class
    bool checkGrid(GridPanel &grid);


    // reset the state
    virtual void reset() = 0;

    // run the algorithm
    // returns true if path found
    virtual bool runComplete(GridPanel &grid) = 0;

    // run the steps necessary for the algorithm to run
    // e.g init data structures
    // returns true if init success
    virtual bool preRun(GridPanel &grid) = 0;

    // run the loop of the algorithm one time
    // return true when done
    virtual bool stepRun(GridPanel &grid) = 0;

    // final steps of the algorithm e.g
    // extract path from beg to end and draw it
    // on the grid
    // returns true if the algorithm found a path
    virtual bool postRun(GridPanel &grid) = 0;

    virtual ~Algorithm();
};

