//
// Created by saad on 6/26/20.
//

#include "MazeRD.h"
#include <stack>
#include <cstdlib>
#include <ctime>

/**
 * To create a maze, we need to define some rules for the grid
 *  The walls can only be made in the odd indices
 *  The paths can only be made in the even indices
 *
 *  https://stackoverflow.com/questions/41553775/maze-generation-recursive-division-how-it-works
 *
 X    0   1   2   3

 x  0 1 2 3 4 5 6 7 8

    # # # # # # # # #   0
    #   +   +   +   #   1  0
    # + # + # + # # #   2
    #   +   +   +   #   3  1
    # + # + # + # # #   4
    #   +   +   +   #   5  2
    # + # + # + # # #   6
    #   +   +   +   #   7  3
    # # # # # # # # #   8

                        y  Y

  *
  * In the above figure, small letters represent the grid coords and big letters represent
  * the maze coords. The algorithm is written using the maze coords
  *
  * The maze size of N x M requires a grid size of (2*N + 1) x (2*M + 1)
  * so given a grid size of R x C, the maze size is ((R - 1) / 2) x ((C - 1) / 2)
  * Note that this only works for odd sizes
  *
  * For even sizes, we need to ceil the result of the division
  * R X C => ceil((R - 1) / 2) ...
  *
  * For our case we define the grid as
  *     walls can only appear on odd indices
  *     drawing a vert. wall at M(0,0) means drawing to the right of G(0,0)
  *     drawing a hori. wall at M(0,0) means drawing below G(0,0)
  *
  * So we can get the following grids
  *
  *     0 1 2 3 4
  *     . # . # .   0       (Odd size)
  *     # # # # #   1
  *     . # . # .   2
  *     # # # # #   3
  *     . # . # .   4
  *
  *     0 1 2 3 4 5
  *     . # . # . #   0     (Even size)
  *     # # # # # #   1
  *     . # . # . #   2
  *     # # # # # #   3
  *     . # . # . #   4
  *     # # # # # #   5
  *
  * Note that the maze size is the same. We can calculate the maze dimensions using
  * Odd  Dimension: [(Dim + 1) / 2]
  * Even Dimension: [Dim / 2]
  *
  * Since we will be writing the algorithm in maze coordinates, we need a way to convert
  * them to grid coords when drawing walls. To do that, note the grid coords are simply
  * maze coords time 2 e.g. M(1, 1) => G(2, 2) in both even and odd size cases. So the wall corresponding to
  * a maze coordinate is just 2*M + 1
  * e.g.    right  wall of M(R,C) is G(2*R, 2*C + 1)    [vertical walls]
  *         bottom wall of M(R,C) is G(2*R + 1, 2*C)    [horizontal walls]
  *
  * Note that right wall of last maze column doesn't exist in odd case. Similarly for the bottom wall
  *
 */

using Rect = GridPanel::Rect;

struct Coord {
    unsigned r, c;
};

struct Region {
    Coord origin, size;
};

static unsigned toMazeDim(unsigned gridDim) {
    if (gridDim % 2 == 0) {
       return gridDim / 2;
    } else {
        return (gridDim + 1) / 2;
    }
}

static Coord findMazeSize(GridPanel &panel) {
    Coord size {};

    size.r = toMazeDim(panel.nRows);
    size.c = toMazeDim(panel.nCols);

    return size;
}

static unsigned genRandom(unsigned min, unsigned max) {
    return min + ((unsigned) rand() % (max - min));
}

static void initRandom() {
  srand(time(nullptr));
}

static bool drawWall(GridPanel &panel, const Region &mazeRegion, const bool hor) {
    const auto &mazeCoords = mazeRegion.origin;
    const auto &mazeSize = mazeRegion.size;

    if (hor) {
        // we draw below the cell
        const auto r = 2 * mazeCoords.r + 1;
        const auto c = 2 * mazeCoords.c;

        // we only need to check if the row is out of range
        // it can only be out of range if nRows is odd
        if (panel.nRows % 2 == 1 && r >= panel.nRows) {
            // no wall here, don't draw
            return false;
        } else {
            const auto width = 2 * mazeSize.c - 1;
            const auto pathC = genRandom(0, width / 2);

            for (auto i = 0u; i < width; ++i) {
                panel.changeRect(panel.rects[r][c + i], RectType::wall);
            }
            panel.changeRect(panel.rects[r][2 * pathC + c], RectType::space);
            return true;
        }
    } else {
        // we draw right of the cell
        const auto r = 2 * mazeCoords.r;
        const auto c = 2 * mazeCoords.c + 1;

        // check if wall exists
        if (panel.nCols % 2 == 1 && c >= panel.nCols) {
            return false;
        } else {
            const auto height = 2 * mazeSize.r - 1;
            const auto pathR  = genRandom(0, height / 2);

            for (auto i = 0u; i < height; ++i) {
                panel.changeRect(panel.rects[r + i][c], RectType::wall);
            }
            panel.changeRect(panel.rects[r + 2 * pathR][c], RectType::space);
            return true;
        }
    }
}

void makeMazeRD(GridPanel &panel) {
    initRandom();
    std::stack<Region> regions;

    panel.clearAll();

    Region startRegion{};

    startRegion.origin  = {0, 0};
    startRegion.size    = findMazeSize(panel);

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
            hor = genRandom(0, 2);
        }

        const auto colOffset = genRandom(0, region.size.c);
        const auto rowOffset = genRandom(0, region.size.r);
        const auto randomColIdx = region.origin.c + colOffset;
        const auto randomRowIdx = region.origin.r + rowOffset;

        Region wall {};
        wall.size = region.size;
        if (hor) {
            wall.origin.r = randomRowIdx;
            wall.origin.c = region.origin.c;
        } else {
            wall.origin.c = randomColIdx;
            wall.origin.r = region.origin.r;
        }
        drawWall(panel, wall, hor);

        // create 2 regions
        Region r1 {}, r2 {};
        if (hor) {
            // the upper region
            r1.origin = region.origin;
            r1.size.c = region.size.c;
            r1.size.r = rowOffset + 1;

            // the lower region
            r2.origin.c = region.origin.c;
            r2.origin.r = randomRowIdx + 1;
            r2.size.c   = region.size.c;
            r2.size.r   = region.size.r - rowOffset - 1;
        } else {
            // the left region
            r1.origin = region.origin;
            r1.size.r = region.size.r;
            r1.size.c = colOffset + 1;

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

