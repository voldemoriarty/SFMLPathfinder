//
// Created by saad on 6/20/20.
//

#include "CtrlPanel.h"
#include "maze/MazeRD.h"
#include "algorithms/All.h"

/*
 * Add algorithm with name to the vector
 * Two vectors are maintained, this is because imgui requires a
 * pointer to zero terminated string of names to appear in a combo box
 */
static void addAlgo(CtrlPanel *panel, Algorithm *alg, const char *name) {
    panel->algs.emplace_back(alg);
    panel->algNames.emplace_back(name);
}

CtrlPanel::CtrlPanel(GridPanel &grid, ImVec2 size, ImVec2 pos)
        :
        grid(grid),
        size(size),
        pos(pos) {
    addAlgo(this, new Bfs, "Breadth First Search");
    addAlgo(this, new Dfs, "Depth First Search");
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
    const ImGuiWindowFlags flags = (unsigned) ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    ImGui::SFML::Update(window, clockTime);
    // set the position of panel
    // run only once; the user can drag and change it
    ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
    ImGui::SetNextWindowSize(size, ImGuiCond_Once);

    ImGui::PushFont(font);
    ImGui::Begin("Control panel", nullptr, flags);

    // row, column sliders
    {
        ImGui::Text("Grid size selectors");

        static int rows_old = (int) grid.nRows, cols_old = (int) grid.nCols;
        static bool same = true;
        int rows = (int) grid.nRows, cols = (int) grid.nCols;
        ImGui::SliderInt("Rows", &rows, 5, 50);
        ImGui::SliderInt("Columns", &cols, 5, 50);
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
        ImGui::Text("Select Rect type to place in grid");
        ImGui::RadioButton("Start Point",(int *) &grid.rectType, 0);
        ImGui::RadioButton("End Point",(int *) &grid.rectType, 1);
        ImGui::RadioButton("Wall", (int *) &grid.rectType, 2);
        ImGui::Separator();
    }

    // algorithm speed selectors
    static int period = 10;
    {
        ImGui::SliderInt("Period (ms)", &period, 0, 1000);
        ImGui::Separator();
    }

    // algorithms
    {
        static bool pathFound = false;
        static bool running = false;
        static bool showAllWindow = false;
        static int algIndex = 0;
        static unsigned tiles = 0;
        static AlgoRunner runner(algs[algIndex].get());

        if (running) {
            tiles = runner.alg->tiles;
            bool done = runner.step(grid);
            if (done) {
                running = false;
                pathFound = runner.post(grid);
            }
        }

        ImGui::Combo("Algorithms", &algIndex, algNames.data(), (int) algNames.size());

        if (ImGui::Button("Run")) {
            pathFound = false;
            showAllWindow = false;
            if (period > 0) {
                runner.setPeriod(sf::milliseconds(period));

                if (running)
                    runner.alg->reset();

                runner.alg = algs[algIndex].get();
                runner.alg->init(grid);
                running = true;
            }
            else {
                if (running)
                    runner.alg->reset();

                pathFound = algs[algIndex]->runComplete(grid);
                tiles = algs[algIndex]->tiles;
                running = false;
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Clear")) {
            grid.clearAll();
            pathFound = false;
            running = false;
            showAllWindow = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Run All")) {
            pathFound = false;
            if (running) {
                runner.alg->reset();
                running = false;
            }
            showAllWindow = true;

            for (auto & alg : algs) {
                alg->runComplete(grid);
            }
        }

        if (showAllWindow) {
            ImGui::Separator();
            for (auto i = 0u; i < algs.size(); ++i) {
                auto &alg = algs[i];
                auto &name = algNames[i];

                ImGui::Text("%s", name);
                ImGui::SameLine(150.0f);
                ImGui::Text("%.4u tiles %4.0f us", alg->tiles, alg->timeUs);
            }
        }

        ImGui::Separator();

        const auto colGreen = ImVec4(0, 1, 0, 1);
        const auto colRed   = ImVec4(1, 0, 0, 1);

        if (!showAllWindow) {
            if (pathFound) {
                ImGui::TextColored(colGreen, "Path found. Tiles checked: %u", tiles);
            } else {
                ImGui::TextColored(colRed, "Path not found. Tiles checked: %u", tiles);
            }
            ImGui::Separator();
        }
    }

    // maze
    {
        static unsigned wallTiles = 0;
        if (ImGui::Button("Random Maze (Recursive Division)")) {
            makeMazeRD(grid);

            // count the number of walls
            wallTiles = 0;
            for (auto & row : grid.rects) {
                wallTiles += std::count_if(row.begin(), row.end(), [](const GridPanel::Rect & rect){
                    return rect.second == RectType::wall;
                });
            }
        }
        ImGui::Text("Wall tiles generated: %u", wallTiles);
        ImGui::Text("Non-wall tiles: %u", grid.nRows * grid.nCols - wallTiles);
        ImGui::Separator();
    }

    ImGui::End();
    ImGui::PopFont();
}

void CtrlPanel::draw(sf::RenderWindow &window) {
    ImGui::SFML::Render(window);
}

CtrlPanel::~CtrlPanel() = default;
