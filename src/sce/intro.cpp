//
// Created by Rudolph on 2023/10/10.
//

#include <iostream>
#include "intro.h"
#include "tetris.h"

Scene::Result Intro::tick(Action action) {
    Scene::tick(action);
    if (ticks >= 120) {
        return Result::next;
    } else {
        if (ticks % 5 == 0) {
            return Result::update;
        }

        return Result::noop;
    }
}

std::vector<Vertex> Intro::getVertices() {
    return {
            {{0.5, 0.5},  2, 1},
            {{0.6, 0.5},  2, 1},
            {{0.5, 0.55}, 2, 1},
            {{0.6, 0.55}, 2, 1},
            {{0.5, 0.55}, 2, 1},
            {{0.6, 0.5},  2, 1}
    };
}

Scene *Intro::nextScene() {
    return new Tetris{};
}
