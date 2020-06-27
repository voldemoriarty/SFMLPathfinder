//
// Created by saad on 6/26/20.
//

#include "AlgoRunner.h"

void AlgoRunner::setPeriod(sf::Time p) {
    period = p;
}

AlgoRunner::AlgoRunner(Algorithm *alg) : alg(alg) {
    assert(alg != nullptr);
    period = sf::Time::Zero;
}

bool AlgoRunner::step(GridPanel &grid) {
    bool done = false;
    if (clk.getElapsedTime() >= period) {
        done = alg->step(grid);
        clk.restart();
    }
    return done;
}

bool AlgoRunner::post(GridPanel &grid) const {
    return alg->postRun(grid);
}
