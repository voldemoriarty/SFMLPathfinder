//
// Created by saad on 6/26/20.
//

#include "AlgoRunner.h"

void AlgoRunner::setPeriod(sf::Time p) {
    period = p;
}

AlgoRunner::AlgoRunner(Algorithm *alg) : alg(alg) {
    assert(alg != nullptr);
    clk.restart();
    period = sf::Time::Zero;
}

bool AlgoRunner::step(GridPanel &grid) {
    bool done = false;
    if (clk.getElapsedTime() >= period) {
        done = alg->stepRun(grid);
        clk.restart();
    }
    return done;
}
