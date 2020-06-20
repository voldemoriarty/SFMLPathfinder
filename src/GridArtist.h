//
// Created by saad on 6/20/20.
//

#ifndef SFMLPATHFINDER_GRIDARTIST_H
#define SFMLPATHFINDER_GRIDARTIST_H

#include <SFML/Graphics.hpp>
#include <vector>

struct GridArtist {

    // all rects in this vector will be drawn
    std::vector<sf::RectangleShape> _rects;

    // grid dimensions
    unsigned nRows, nCols;

    // size of each rect
    sf::Vector2f size;

    // the window size info
    sf::VideoMode vidMode;

    // origin of the grid
    // this acts as the (0,0)
    sf::Vector2f origin;

    GridArtist(unsigned nRows, unsigned nCols, sf::VideoMode windowDim, sf::Vector2f origin = sf::Vector2f(0, 0));

    void changeRC(unsigned deltaR, unsigned deltaC);

    void incCols();

    void incRows();

    void decCols();

    void decRows();

    void resize(const sf::VideoMode &newSize);

    void draw(sf::RenderWindow& window);

    static void toggleRect(sf::RectangleShape &rect);

    void mouseHandle(sf::RenderWindow &window);

    void kbKeyRelHandle(sf::Event &e);
};


#endif //SFMLPATHFINDER_GRIDARTIST_H
