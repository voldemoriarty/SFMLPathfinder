//
// Created by saad on 6/20/20.
//

#include "GridArtist.h"

GridArtist::GridArtist(const unsigned int nRows, const unsigned int nCols, sf::VideoMode windowDim, sf::Vector2f origin)
        :
        nRows(nRows),
        nCols(nCols),
        vidMode(windowDim),
        origin(origin),
        _rects(std::vector<RowVector> {}) {
    resize(windowDim);
}

void GridArtist::changeRC(const unsigned int deltaR, const unsigned int deltaC) {
    nCols += deltaC;
    nRows += deltaR;
    resize(vidMode);
}

void GridArtist::incCols() {
    changeRC(0, 1);
}

void GridArtist::incRows() {
    changeRC(1, 0);
}

void GridArtist::decCols() {
    if (nCols != 0) {
        changeRC(0, -1);
    }
}

void GridArtist::decRows() {
    if (nRows != 0) {
        changeRC(-1, 0);
    }
}

void GridArtist::resize(const sf::VideoMode &newSize) {
    size = sf::Vector2f((float) newSize.width / (float) nCols, (float) newSize.height / (float) nRows);

    // clear the rects from the vector
    _rects.clear();
    oldRect = nullptr;

    // set the outline percentage (of width)
    const float outline = 0.05f * size.x;

    for (auto i = 0; i < nRows; ++i) {
        _rects.emplace_back(RowVector{});
        for (auto j = 0; j < nCols; ++j) {
            sf::RectangleShape rect;
            sf::Vector2f pos(i * size.x, j * size.y);

            rect.setSize(size);
            rect.setFillColor(sf::Color::Blue);
            rect.setOutlineThickness(-outline);
            rect.setOutlineColor(sf::Color::Black);

            // set the position in the matrix
            // add an offset to the origin
            rect.setPosition(pos + origin);

            _rects[i].emplace_back(rect);
        }
    }
}

void GridArtist::draw(sf::RenderWindow &window) {
    // draw them rects
    for (auto& row : _rects) {
        for (auto& rect : row) {
            window.draw(rect);
        }
    }
}

void GridArtist::toggleRect(sf::RectangleShape &rect) {
    if (rect.getFillColor() == sf::Color::Blue) {
        rect.setFillColor(sf::Color::Yellow);
    }
    else {
        rect.setFillColor(sf::Color::Blue);
    }
}

void GridArtist::mouseHandle(sf::RenderWindow &window) {
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
    auto rowIdx = (unsigned) mousePos.x / (unsigned) size.x;
    auto colIdx = (unsigned) mousePos.y / (unsigned) size.y;

    // need to remember the previous location of the mouse
    // so that we can turn its outline back to black when mouse
    // leaves it

    if (rowIdx < nRows && colIdx < nCols) {
        auto *rect = &_rects[rowIdx][colIdx];
        rect->setOutlineColor(sf::Color::Green);
        if (oldRect != nullptr && oldRect != rect) {
            oldRect->setOutlineColor(sf::Color::Black);
        }
        oldRect = rect;
        if (mouseClickEdge) {
            toggleRect(*rect);
        }
    }
}

void GridArtist::kbKeyRelHandle(sf::Event &e) {
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
