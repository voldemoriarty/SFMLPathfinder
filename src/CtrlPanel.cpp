//
// Created by saad on 6/20/20.
//

#include "CtrlPanel.h"

CtrlPanel::CtrlPanel(GridPanel &grid, ImVec2 size, ImVec2 pos)
        :
        grid(grid),
        size(size),
        pos(pos) {
}

void CtrlPanel::init(sf::RenderWindow &window, const char *fileName, bool lightTheme) {
    ImGui::SFML::Init(window);
    if (lightTheme) {
        ImGui::StyleColorsLight();
    }

    if (fileName != nullptr) {
        font = ImGui::GetIO().Fonts->AddFontFromFileTTF(fileName, 18.0f);
        ImGui::SFML::UpdateFontTexture();
    }

    if (font == nullptr) {
        std::cerr << "Couldn't load font: " << fileName << std::endl;
    }
}

void CtrlPanel::loop(sf::Time clockTime, sf::RenderWindow &window) const {
    ImGui::SFML::Update(window, clockTime);
    // set the position of panel
    // run only once; the user can drag and change it
    ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
    ImGui::SetNextWindowSize(size, ImGuiCond_Once);

    ImGui::Begin("Control panel");
    ImGui::PushFont(font);
    ImGui::Button("I'm a button");
    ImGui::PopFont();
    ImGui::End();
}

void CtrlPanel::draw(sf::RenderWindow &window) {
    ImGui::SFML::Render(window);
}
