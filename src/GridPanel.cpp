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
    oldRect = nullptr;

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

void GridPanel::toggleRect(Rect &rect, RectType to) const {
    sf::Color newColor;
    switch (to) {
        case RectType::wall:
            newColor = sf::Color::Red;
            break;
        case RectType::start:
            newColor = sf::Color::Green;
            break;
        case RectType::end:
            newColor = sf::Color::Yellow;
            break;
        case RectType::space:
            newColor = sf::Color::Blue;
            break;
    }

    if (rect.second != rectType) {
        rect.first.setFillColor(newColor);
        rect.second = rectType;
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

RectType GridPanel::getRectType(unsigned int row, unsigned int col) {
    assert(row < nRows);
    assert(col < nCols);
    return rects[row][col].second;
}
