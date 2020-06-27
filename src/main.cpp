#include <SFML/Graphics.hpp>
#include "GridPanel.h"
#include "CtrlPanel.h"

int main()
{
    const unsigned nRows = 15;
    const unsigned nCols = 15;

    const unsigned gridW = 600;
    const unsigned ctrlW = 300;
    const unsigned windH = 600;

    sf::VideoMode       gridMode(gridW, windH);
    sf::RenderWindow    window(sf::VideoMode(gridW + ctrlW, windH), "PathFinder");

    GridPanel grid(nRows, nCols, gridMode);
    CtrlPanel ctrl(grid, sf::Vector2f(ctrlW, windH), sf::Vector2f(gridW, 0));
    bool inFocus = true;

    ctrl.init(window, "freesans.ttf", false);

    sf::Clock deltaClock;

    // lock at 30fps
    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::GainedFocus) {
                inFocus = true;
            }
            else if (event.type == sf::Event::LostFocus) {
                inFocus = false;
            }
            else if (event.type == sf::Event::KeyReleased) {
                grid.kbKeyRelHandle(event);
            }
        }

        if (inFocus) {
            grid.mouseHandle(window);
        }

        ctrl.loop(deltaClock.restart(), window);
        window.clear();
        grid.draw(window);
        CtrlPanel::draw(window);
        window.display();
    }

    for (auto & row : grid.rects) {
        for (auto & pair : row) {
            char c;
            switch (pair.second) {
                case RectType::space:   c = '.'; break;
                case RectType::wall:    c = '#'; break;
                case RectType::start:   c = 'O'; break;
                case RectType::end:     c = 'X'; break;
                case RectType::path:    c = '='; break;
            }
            std::cout << c << ' ';
        }
        std::cout << std::endl;
    }
    return 0;
}
