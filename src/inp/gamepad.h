//
// Created by Rudolph on 2023/08/02.
//

#ifndef VULKAN_TETRIS_GAMEPAD_H
#define VULKAN_TETRIS_GAMEPAD_H


#include "GLFW/glfw3.h"
#include "../structs.h"

#define GAMEPAD_RELEASE                0
#define GAMEPAD_PRESS                  1
#define GAMEPAD_HOLD                   10
#define GAMEPAD_N_BUTTONS              15

class Gamepad {
    enum Buttons {
        a = 0,
        b,
        x,
        y,
        left_bumper,
        right_bumper,
        back,
        start,
        guide,
        left_thumb,
        right_thumb,
        up,
        right,
        down,
        left
    };
private:
    static int buttons[GAMEPAD_N_BUTTONS];
    static int eventButtons[GAMEPAD_N_BUTTONS];

    static int currentGamepad;
    static GLFWgamepadstate state;
    static bool gamepadConnected;

public:
    static void gamepadCallback(int jid, int event);

    static void tick();

    static void updateAction(Action &oldAction);

};


#endif //VULKAN_TETRIS_GAMEPAD_H
