//
// Created by saad on 6/26/20.
//

#include "MazeRD.h"
#include <stack>
#include <cstdlib>
#include <ctime>

using Rect = GridPanel::Rect;

struct Coord {
    unsigned r, c;
};

struct Region {
    Coord origin, size;
};

void makeMazeRD(GridPanel &panel) {
    srandom(time(nullptr));
    std::stack<Region> regions;

    panel.clearAll();

    Region startRegion {
        .origin={0, 0},
        .size={panel.nRows, panel.nCols}
    };

    regions.push(startRegion);
    while (!regions.empty()) {
        auto region = regions.top();
        regions.pop();

        // exit condition
        if (region.size.r < 2 || region.size.c < 2)
            continue;

        // create a random wall
        bool hor;

        if (region.size.c < region.size.r) {
            hor = true;
        }
        else if (region.size.r < region.size.c){
            hor = false;
        }
        else {
            hor = random() % 2 == 0;
        }

        const auto colOffset = random() % (region.size.c);
        const auto rowOffset = random() % (region.size.r);
        const auto randomColIdx = region.origin.c + colOffset;
        const auto randomRowIdx = region.origin.r + rowOffset;

        if (hor) {
            for (auto i = 0; i < region.size.c; ++i) {
                panel.changeRect(panel.rects[randomRowIdx][i + region.origin.c], RectType::wall);
            }

            // find a random opening in the wall
            panel.changeRect(panel.rects[randomRowIdx][randomColIdx], RectType::space);
        }
        else {
            for (auto i = 0; i < region.size.r; ++i) {
                panel.changeRect(panel.rects[i + region.origin.r][randomColIdx], RectType::wall);
            }

            // find a random opening
            panel.changeRect(panel.rects[randomRowIdx][randomColIdx], RectType::space);
        }

        // create 2 regions
        Region r1 {}, r2 {};
        if (hor) {
            // the upper region
            r1.origin = region.origin;
            r1.size.c = region.size.c;
            r1.size.r = rowOffset;

            // the lower region
            r2.origin.c = region.origin.c;
            r2.origin.r = randomRowIdx + 1;
            r2.size.c   = region.size.c;
            r2.size.r   = region.size.r - rowOffset - 1;
        }
        else {
            // the left region
            r1.origin = region.origin;
            r1.size.r = region.size.r;
            r1.size.c = colOffset;

            // the right region
            r2.origin.r = region.origin.r;
            r2.origin.c = randomColIdx + 1;
            r2.size.r   = region.size.r;
            r2.size.c   = region.size.c - colOffset - 1;
        }

        regions.push(r1);
        regions.push(r2);
    }
}

