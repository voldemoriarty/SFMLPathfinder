//
// Created by saad on 6/20/20.
//

#pragma once

enum class RectType : int {
    start = 0,  // start node
    end,        // end node
    wall,       // blocking node
    space,      // empty space
    path        // path from source to dest
};
