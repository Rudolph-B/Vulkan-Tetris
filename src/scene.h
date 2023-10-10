//
// Created by Rudolph on 2023/10/10.
//

#ifndef VULKAN_TETRIS_SCENE_H
#define VULKAN_TETRIS_SCENE_H

#include "structs.h"

class Scene {
public:
    enum Result {
        noop, update, next, stop, close
    };

    virtual Result tick(Action action);
    virtual std::vector<Vertex> getVertices();
    virtual Scene* nextScene();

    /**
     * @brief A struct to hold various Keyboard input
     */
protected:
    // Keeps track of the number of ticks since the game started
    int ticks{};
};


#endif //VULKAN_TETRIS_SCENE_H
