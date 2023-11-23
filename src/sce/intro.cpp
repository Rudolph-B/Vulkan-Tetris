//
// Created by Rudolph on 2023/10/10.
//

#include <iostream>
#include <map>
#include <tinyxml2.h>
#include "intro.h"
#include "tetris.h"
#include "glm/glm.hpp"
#include "../tls/svg.h"

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
    return vertices;
}

Scene *Intro::nextScene() {
    return new Tetris{};
}

Intro::Intro() {
    std::unordered_map<std::string, std::vector<Vertex>> refVertexes;
    Svg svgSource("ass/intro.o.svg");

    refVertexes = svgSource.getReferenceVertexes();
    vertices = svgSource.populateDisplayVertexes(refVertexes);
}