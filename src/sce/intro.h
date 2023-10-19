//
// Created by Rudolph on 2023/10/10.
//

#ifndef VULKAN_TETRIS_INTRO_H
#define VULKAN_TETRIS_INTRO_H


#include "../scene.h"

class Intro: public Scene {
public:
    Intro();

private:
    Result tick(Action action) override;

    std::vector<Vertex> getVertices() override;

    Scene* nextScene() override;
};


#endif //VULKAN_TETRIS_INTRO_H
