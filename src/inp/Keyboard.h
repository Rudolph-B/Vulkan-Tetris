//
// Created by Rudolph on 2023/08/01.
//

#ifndef VULKAN_TETRIS_KEYBOARD_H
#define VULKAN_TETRIS_KEYBOARD_H


#include "GLFW/glfw3.h"
#include "../structs.h"
#include <iostream>

#define KEYBOARD_RELEASE                0
#define KEYBOARD_PRESS                  1
#define KEYBOARD_HOLD                   15
#define KEYBOARD_N_KEYS                 350

class Keyboard {
private:
    static int keys[KEYBOARD_N_KEYS];
    static int eventKeys[KEYBOARD_N_KEYS];

public:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static Action getAction();
    static void tick();
};

#endif //VULKAN_TETRIS_KEYBOARD_H
