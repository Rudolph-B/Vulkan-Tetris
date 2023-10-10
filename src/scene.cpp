//
// Created by Rudolph on 2023/10/10.
//

#include "scene.h"

Scene::Result Scene::tick(Action action) {
    ticks++;
    return Result::close;
}

std::vector<Vertex> Scene::getVertices() {
    return {};
}

Scene* Scene::nextScene() {
    return new Scene();
}
