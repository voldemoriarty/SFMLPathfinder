//
// Created by saad on 6/20/20.
//

#ifndef SFMLPATHFINDER_GRIDPANEL_H
#define SFMLPATHFINDER_GRIDPANEL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include <cassert>
#include "RectType.h"

struct GridPanel {
    // helper to create a 2D vector
    using Rect      = std::pair<sf::RectangleShape, RectType>;
    using RowVector = std::vector<Rect>;

    // all rects in this vector will be drawn
    std::vector<RowVector> rects;

    // the previous rect mouse was hovering on
    sf::RectangleShape *oldRect = nullptr;

    // grid dimensions
    unsigned nRows, nCols;

    // size of each rect
    sf::Vector2f size;

    // the window size info
    sf::VideoMode vidMode;

    // origin of the grid
    // this acts as the (0,0)
    sf::Vector2f origin;

    // these are input from the control panel
    // these control the color of the rect which is
    // clicked
    RectType rectType = RectType::wall;

    /*
     * Create a grid of specified rows and cols divided evenly in a window of size
     * windowDim and starting position origin
     */
    GridPanel(unsigned nRows, unsigned nCols, sf::VideoMode windowDim, sf::Vector2f origin = sf::Vector2f(0, 0));

    /*
     * Increase row and cols by given amount
     * put -ve values to decrease
     */
    void changeRC(unsigned deltaR, unsigned deltaC);

    /*
     * Increment cols by 1
     */
    void incCols();

    /*
     * Increment rows by 1
     */
    void incRows();

    /*
     * decrement cols by 1
     */
    void decCols();

    /*
     * decrement rows by 1
     */
    void decRows();

    /*
     * resize the rects to fit in new size.
     */
    void resize(const sf::VideoMode &newSize);

    /*
     * Draw all rects to the render window
     */
    void draw(sf::RenderWindow& window);

    /*
     * Toggle rect color against selected type in
     * the control panel
     */
    void toggleRect(Rect &rect, RectType to) const;

    /*
     * Handle mouse in the window
     */
    void mouseHandle(sf::RenderWindow &window);

    /*
     * Handle keyboard key release event
     */
    void kbKeyRelHandle(sf::Event &e);

    /*
     * Get the type of a rect in the grid
     */
    RectType getRectType(unsigned row, unsigned col);

    /*
     * Set the rect type
     */
    void setRectType(unsigned row, unsigned col, RectType type);
};

#endif //SFMLPATHFINDER_GRIDPANEL_H
