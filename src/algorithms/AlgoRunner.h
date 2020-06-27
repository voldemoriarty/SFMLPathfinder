//
// Created by saad on 6/26/20.
//

#pragma once

#include "Algorithm.h"

struct AlgoRunner {
    Algorithm   *alg = nullptr;
    sf::Clock   clk;
    sf::Time    period;

    explicit AlgoRunner(Algorithm *alg);

    void setPeriod(sf::Time p);

    bool step(GridPanel &grid);

    bool post(GridPanel &grid) const;
};

