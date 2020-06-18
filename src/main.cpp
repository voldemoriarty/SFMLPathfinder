#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>

// draw a simple grid with rects

struct Grid {

    // all rects in this vector will be drawn
    std::vector<sf::RectangleShape> _rects;

    // grid dimensions
    unsigned nRows, nCols;

    // size of each rect
    sf::Vector2f size;

    Grid(const unsigned nRows, const unsigned nCols, const sf::VideoMode windowDim) : nRows(nRows), nCols(nCols), _rects(nRows* nCols) {
        // make each rectangle of same color
        // and size

        // window width = nCols * (rectWidth)
        // same for height

        // calculate size of single rectangle
        size = sf::Vector2f(windowDim.width / nCols, windowDim.height / nRows);

        // set the outline percentage (of width)
        const float outline = 0.05 * size.x;

        for (auto i = 0; i < nRows; ++i) {
            for (auto j = 0; j < nCols; ++j) {
                sf::RectangleShape rect;
                rect.setSize(size);
                rect.setFillColor(sf::Color::Blue);
                rect.setOutlineThickness(-outline);
                rect.setOutlineColor(sf::Color::Black);

                // set the position in the matrix
                rect.setPosition(i * size.x, j * size.y);

                // add the rects to the drawing vector
                _rects.push_back(rect);
            }
        }

    }

    void draw(sf::RenderWindow& window) {
        window.clear();

        // draw them rects
        for (auto& rect : _rects) {
            window.draw(rect);
        }
        window.display();
    }

    void toggleRect(sf::RectangleShape &rect) {    
        if (rect.getFillColor() == sf::Color::Blue) {
            rect.setFillColor(sf::Color::Yellow);
        }
        else {
            rect.setFillColor(sf::Color::Blue);
        } 
    }

    void mouseHandle(const sf::Vector2f pos, bool clicked) {
        for (auto& rect : _rects) {
            bool inside = rect.getGlobalBounds().contains(pos);
            if (inside) {
                rect.setOutlineColor(sf::Color::Green);
                if (clicked) {
                    toggleRect(rect);
                }
            }
            else {
                rect.setOutlineColor(sf::Color::Black);
            }
        }
    }
};

int main()
{
    const unsigned nRows = 5;
    const unsigned nCols = 5;

    sf::VideoMode     mode(600, 600);
    sf::RenderWindow  window(mode, "Grid");

    Grid grid(nRows, nCols, mode);

    bool isMousePressed = false;
    bool inFocus = false;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::GainedFocus)
                inFocus = true;
            else if (event.type == sf::Event::LostFocus)
                inFocus = false;
        }

        if (inFocus) {
            // mouse pos rel to window
            auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // implement edge detection on mouse clicks
            // since sfml doesnt provide it

            bool mouseClickEdge = false;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (!isMousePressed) {
                    // at this point the edge of the click occured
                    // handle it
                    mouseClickEdge = true;
                    isMousePressed = true;
                }
            }
            else {
                isMousePressed = false;
            }

            // handle mouse hover
            grid.mouseHandle(mousePos, mouseClickEdge);
        }
        // draw
        grid.draw(window);
    }

    return 0;
}
