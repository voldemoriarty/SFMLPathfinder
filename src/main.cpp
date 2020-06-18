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

    // the window size info
    sf::VideoMode vidMode;

    Grid(const unsigned nRows, const unsigned nCols, const sf::VideoMode windowDim) : 
    nRows(nRows), 
    nCols(nCols), 
    vidMode(windowDim),
    _rects(nRows* nCols) {
        resize(windowDim);
    }

    void changeRC(const unsigned deltaR, const unsigned deltaC) {
        nCols += deltaC;
        nRows += deltaR;
        resize(vidMode);
    }

    void incCols() {
        changeRC(0, 1);
    }

    void incRows() {
        changeRC(1, 0);
    }

    void decCols() {
        if (nCols != 0) {
            changeRC(0, -1);
        }
    }

    void decRows() {
        if (nRows != 0) {
            changeRC(-1, 0);
        }
    }
 
    void resize(const sf::VideoMode &newSize) {
        size = sf::Vector2f((float) newSize.width / nCols, (float) newSize.height / nRows);

        // clear the rects from the vector
        _rects.clear();

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

    void mouseHandle(sf::RenderWindow &window) {
        static bool isMousePressed = false;

        // mouse pos rel to window
        auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // implement edge detection on mouse clicks
        // since sfml doesnt provide it
        bool mouseClickEdge = false;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!isMousePressed) {
                // at this point the edge of the click occured
                mouseClickEdge = true;
                isMousePressed = true;
            }
        }
        else {
            isMousePressed = false;
        }

        for (auto& rect : _rects) {
            bool inside = rect.getGlobalBounds().contains(mousePos);
            if (inside) {
                rect.setOutlineColor(sf::Color::Green);
                if (mouseClickEdge) {
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
    const unsigned nRows = 15;
    const unsigned nCols = 15;

    const unsigned gridW = 600;
    const unsigned ctrlW = 300;
    const unsigned windH = 600;

    sf::VideoMode       gridMode(gridW, windH);
    sf::VideoMode       ctrlMode(ctrlW, windH);
    sf::RenderWindow    window(sf::VideoMode(gridW + ctrlW, windH), "Grid with Control Space");

    Grid grid(nRows, nCols, gridMode);

    bool isMousePressed = false;
    bool inFocus = false;

    // lock at 30fps
    window.setFramerateLimit(30);

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
            else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Add) {
                grid.incCols();
                grid.incRows();
            }
            else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Subtract) {
                grid.decCols();
                grid.decRows();
            }
        }

        if (inFocus) {
            grid.mouseHandle(window);
        }

        grid.draw(window);
    }

    return 0;
}
