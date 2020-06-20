#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>
#include "GridArtist.h"

int main()
{
    const unsigned nRows = 15;
    const unsigned nCols = 15;

    const unsigned gridW = 600;
    const unsigned ctrlW = 300;
    const unsigned windH = 600;

    sf::VideoMode       gridMode(gridW, windH);
    sf::RenderWindow    window(sf::VideoMode(gridW + ctrlW, windH), "Grid with Control Space");

    GridArtist grid(nRows, nCols, gridMode);

    bool inFocus = true;

    ImGui::SFML::Init(window);
    ImGui::StyleColorsLight();

    // init fonts
    auto& io    = ImGui::GetIO();
    auto font   = io.Fonts->AddFontFromFileTTF("freesans.ttf", 20.0f);
    ImGui::SFML::UpdateFontTexture();

    if (font == nullptr) {
        std::cerr << "Error loading font. Using default" << std::endl;
    }

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

        ImGui::SFML::Update(window, deltaClock.restart());
        
        ImGui::SetNextWindowPos(sf::Vector2f(gridW, 0), ImGuiCond_Once);
        ImGui::SetNextWindowSize(sf::Vector2f(ctrlW, windH), ImGuiCond_Once);

        ImGui::Begin("Control panel");
        ImGui::PushFont(font);
        ImGui::Button("I'm a button");
        ImGui::PopFont();
        ImGui::End();

        window.clear();

        grid.draw(window);
        ImGui::SFML::Render(window);
        
        window.display();
    }

    return 0;
}
