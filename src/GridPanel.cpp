//
// Created by saad on 6/20/20.
//

#include "GridPanel.h"

GridPanel::GridPanel(const unsigned int nRows, const unsigned int nCols, sf::VideoMode windowDim, sf::Vector2f origin)
        :
        nRows(nRows),
        nCols(nCols),
        vidMode(windowDim),
        origin(origin),
        rects(std::vector<RowVector> {}) {
    resize(windowDim);
}

void GridPanel::changeRC(const unsigned int deltaR, const unsigned int deltaC) {
    nCols += deltaC;
    nRows += deltaR;
    resize(vidMode);
}

void GridPanel::incCols() {
    changeRC(0, 1);
}

void GridPanel::incRows() {
    changeRC(1, 0);
}

void GridPanel::decCols() {
    if (nCols != 0) {
        changeRC(0, -1);
    }
}

void GridPanel::decRows() {
    if (nRows != 0) {
        changeRC(-1, 0);
    }
}

void GridPanel::resize(const sf::VideoMode &newSize) {
    size = sf::Vector2f((float) newSize.width / (float) nCols, (float) newSize.height / (float) nRows);

    // clear the rects from the vector
    rects.clear();
    oldRect     = nullptr;
    startPoint  = nullptr;
    endPoint    = nullptr;

    // set the outline percentage (of width)
    const float outline = 0.05f * size.x;

    for (auto i = 0; i < nRows; ++i) {
        rects.emplace_back(RowVector{});
        for (auto j = 0; j < nCols; ++j) {
            sf::RectangleShape rect;
            sf::Vector2f pos(j * size.x, i * size.y);

            rect.setSize(size);
            rect.setFillColor(sf::Color::Blue);
            rect.setOutlineThickness(-outline);
            rect.setOutlineColor(sf::Color::Black);

            // set the position in the matrix
            // add an offset to the origin
            rect.setPosition(pos + origin);

            rects[i].emplace_back(std::make_pair(rect, RectType::space));
        }
    }
}

void GridPanel::draw(sf::RenderWindow &window) {
    // draw them rects
    for (auto& row : rects) {
        for (auto& rect : row) {
            window.draw(rect.first);
        }
    }
}

void GridPanel::toggleRect(Rect &rect, RectType to) {
    sf::Color newColor;
    switch (to) {
        case RectType::wall:
            newColor = sf::Color::Red;
            break;
        case RectType::start:
            newColor = sf::Color::Green;
            if (startPoint != nullptr) {
                toggleRect(*startPoint, RectType::space);
            }
            startPoint = &rect;
            break;
        case RectType::end:
            newColor = sf::Color::Yellow;
            if (endPoint != nullptr) {
                toggleRect(*endPoint, RectType::space);
            }
            endPoint = &rect;
            break;
        case RectType::space:
            newColor = sf::Color::Blue;
            break;
        case RectType::path:
            newColor = sf::Color::Cyan;
            break;
    }

    if (rect.second != to) {
        rect.first.setFillColor(newColor);
        rect.second = to;
    }
    else {
        rect.first.setFillColor(sf::Color::Blue);
        rect.second = RectType::space;
    }
}

void GridPanel::mouseHandle(sf::RenderWindow &window) {
    static bool isMousePressed = false;

    // mouse pos rel to window
    auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // implement edge detection on mouse clicks
    // since sfml doesnt provide it
    bool mouseClickEdge = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!isMousePressed) {
            // at this point the edge of the click occurred
            mouseClickEdge = true;
            isMousePressed = true;
        }
    }
    else {
        isMousePressed = false;
    }

    // find the rect in which the mouse cursor lies
    auto colIdx = (unsigned) mousePos.x / (unsigned) size.x;
    auto rowIdx = (unsigned) mousePos.y / (unsigned) size.y;

    // need to remember the previous location of the mouse
    // so that we can turn its outline back to black when mouse
    // leaves it

    if (rowIdx < nRows && colIdx < nCols) {
        auto &pair = rects[rowIdx][colIdx];
        auto *rect = &pair.first;
        auto &type = pair.second;

        rect->setOutlineColor(sf::Color::Green);
        if (oldRect != nullptr && oldRect != rect) {
            oldRect->setOutlineColor(sf::Color::Black);
        }
        oldRect = rect;
        if (mouseClickEdge) {
            // toggle the rect that was clicked. rectType is from
            // the control panel. It tells to toggle to a wall, start, end etc...
            toggleRect(pair, rectType);
        }
    } else {
        if (oldRect != nullptr) {
            oldRect->setOutlineColor(sf::Color::Black);
        }
    }
}

void GridPanel::kbKeyRelHandle(sf::Event &e) {
    switch (e.key.code) {
        case sf::Keyboard::Add:
            incCols();
            incRows();
            break;

        case sf::Keyboard::Subtract:
            decCols();
            decRows();
            break;

        default:
            break;
    }
}

GridPanel::Idx GridPanel::rectToIdx(GridPanel::Rect *rect) {
    auto& shape = rect->first;
    Idx index;
    index.second = shape.getPosition().x / shape.getSize().x;
    index.first  = shape.getPosition().y / shape.getSize().y;
    return index;
}

GridPanel::Rect* GridPanel::findNeighbour(GridPanel::Rect *src, int dir) {
    auto [row, col] = rectToIdx(src);
    Rect *ret;

    switch (dir) {
        case 0:
            if (row == 0)
                ret = nullptr;
            else
                ret = &rects[row - 1][col];
            break;
        case 1:
            if (col == (nCols - 1))
                ret =  nullptr;
            else
                ret =  &rects[row][col + 1];
            break;
        case 2:
            if (row == (nRows - 1))
                ret = nullptr;
            else
                ret = &rects[row + 1][col];
            break;
        case 3:
            if (col == 0)
                ret = nullptr;
            else
                ret = &rects[row][col - 1];
            break;
        default:
            ret = nullptr;
            break;
    }
    return ret;
}

void GridPanel::changeRect(GridPanel::Rect &rect, RectType to) {
    if (rect.second != to) {
        toggleRect(rect, to);
    }
}

void GridPanel::clearAllPaths() {
    oldFocus = nullptr;
    for (auto & row: rects) {
        for (auto & rect : row) {
            if (rect.second == RectType::path)
                changeRect(rect, RectType::space);
            rect.first.setOutlineColor(sf::Color::Black);
        }
    }
}

void GridPanel::clearAll() {
    startPoint  = nullptr;
    endPoint    = nullptr;
    oldFocus    = nullptr;
    for (auto & row: rects) {
        for (auto & rect : row) {
            changeRect(rect, RectType::space);
            rect.first.setOutlineColor(sf::Color::Black);
        }
    }
}

void GridPanel::putInFocus(GridPanel::Rect *rect) {
    rect->first.setOutlineColor(sf::Color::Cyan);
    if (oldFocus != nullptr && oldFocus != rect) {
        oldFocus->first.setOutlineColor(sf::Color::Black);
    }
    oldFocus = rect;
}

void GridPanel::clearFocus() {
    if (oldFocus != nullptr) {
        oldFocus->first.setOutlineColor(sf::Color::Black);
        oldFocus = nullptr;
    }
}
