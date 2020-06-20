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

    ImGui::PushFont(font);
    ImGui::Begin("Control panel");

    // row, column sliders
    {
        ImGui::Text("Grid size selectors");

        static int rows_old = (int) grid.nRows, cols_old = (int) grid.nCols;
        static bool same = true;
        int rows = (int) grid.nRows, cols = (int) grid.nCols;
        ImGui::SliderInt("Rows", &rows, 3, 50);
        ImGui::SliderInt("Columns", &cols, 3, 50);
        ImGui::Checkbox("Same", &same);

        bool colChanged = cols != cols_old;
        bool rowChanged = rows != rows_old;

        if (same & rowChanged) cols = rows;
        if (same & colChanged) rows = cols;

        if (rowChanged || colChanged && (rows > 0 && cols > 0)) {
            grid.changeRC(rows - grid.nRows, cols - grid.nCols);
            rows_old = rows;
            cols_old = cols;
        }

        ImGui::Separator();
    }

    // starting and ending points
    {
        ImGui::Text("Select Rect Type");
        ImGui::RadioButton("Start",(int *) &grid.rectType, 0);
        ImGui::RadioButton("End",(int *) &grid.rectType, 1);
        ImGui::RadioButton("Wall", (int *) &grid.rectType, 2);
        ImGui::Separator();
    }

    ImGui::End();
    ImGui::PopFont();
}

void CtrlPanel::draw(sf::RenderWindow &window) {
    ImGui::SFML::Render(window);
}
