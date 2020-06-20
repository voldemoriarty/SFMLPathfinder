//
// Created by saad on 6/20/20.
//

#ifndef SFMLPATHFINDER_RECTTYPE_H
#define SFMLPATHFINDER_RECTTYPE_H

enum class RectType : int {
    start = 0,  // start node
    end,        // end node
    wall,       // blocking node
    space       // empty space
};

#endif //SFMLPATHFINDER_RECTTYPE_H
