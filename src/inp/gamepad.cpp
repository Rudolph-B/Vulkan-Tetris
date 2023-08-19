//
// Created by Rudolph on 2023/08/02.
//

#include <iostream>
#include "gamepad.h"

int Gamepad::buttons[GAMEPAD_N_BUTTONS] = {0};
int Gamepad::eventButtons[GAMEPAD_N_BUTTONS] = {0};
int Gamepad::currentGamepad;
bool Gamepad::gamepadConnected;
GLFWgamepadstate Gamepad::state;

void Gamepad::gamepadCallback(int jid, int event) {
    if (event == GLFW_CONNECTED)
    {
        std::cout << "Gamepad connected: " << glfwGetJoystickName(jid) << std::endl;
        glfwJoystickIsGamepad(jid);
        currentGamepad = jid;
        gamepadConnected = true;
        if (!(glfwGetGamepadState(currentGamepad, &state))) {
            std::cout << "Unable to read gamepad: " << std::endl;
        }
    }
    else if (event == GLFW_DISCONNECTED)
    {
        std::cout << "Gamepad disconnected: " << glfwGetJoystickName(jid) << std::endl;
    }
}


void Gamepad::tick() {
    GLFWgamepadstate nState;
    if (!(glfwGetGamepadState(currentGamepad, &nState)) && gamepadConnected) {
        std::cout << "Unable to read gamepad: " << std::endl;
    }

    // Loop through all buttons
    for (int i = 0; i < GAMEPAD_N_BUTTONS; i++) {
        if (state.buttons[i] != nState.buttons[i]) {
            if (nState.buttons[i] == GLFW_PRESS) {
                Gamepad::buttons[i] = GAMEPAD_PRESS;
                Gamepad::eventButtons[i] = GAMEPAD_PRESS;
            }
            else if (nState.buttons[i] == GLFW_RELEASE) {
                Gamepad::buttons[i] = GAMEPAD_RELEASE;
                Gamepad::eventButtons[i] = GAMEPAD_RELEASE;
            }
        }
        else {
            if (nState.buttons[i] == GLFW_PRESS) {
                if (Gamepad::eventButtons[i] >= GAMEPAD_PRESS && Gamepad::eventButtons[i] < GAMEPAD_HOLD) {
                    Gamepad::eventButtons[i] += GAMEPAD_PRESS;
                }
            }
        }

        state.buttons[i] = nState.buttons[i];
    }
}

void Gamepad::updateAction(Action &oldAction) {
    oldAction.left = (Gamepad::buttons[left] == GAMEPAD_PRESS) ? press : oldAction.left;
    oldAction.right = (Gamepad::buttons[right] == GAMEPAD_PRESS) ? press : oldAction.right;
    oldAction.down = (Gamepad::buttons[down] == GAMEPAD_PRESS) ? press : oldAction.down;
    oldAction.rotateLeft = (Gamepad::buttons[up] == GAMEPAD_PRESS) ? press : oldAction.rotateLeft;
    oldAction.drop = (Gamepad::buttons[x] == GAMEPAD_PRESS) ? press : oldAction.drop;
    oldAction.pageUp = (Gamepad::buttons[right_bumper] == GAMEPAD_PRESS) ? press : oldAction.pageUp;
    oldAction.pageDown = (Gamepad::buttons[left_bumper] == GAMEPAD_PRESS) ? press : oldAction.pageDown;
    oldAction.enter = (Gamepad::buttons[start] == GAMEPAD_PRESS) ? press : oldAction.enter;
    oldAction.esc = (Gamepad::buttons[back] == GAMEPAD_PRESS) ? press : oldAction.esc;

    oldAction.left = (Gamepad::eventButtons[left] == GAMEPAD_HOLD) ? hold : oldAction.left;
    oldAction.right = (Gamepad::eventButtons[right] == GAMEPAD_HOLD) ? hold : oldAction.right;
    oldAction.down = (Gamepad::eventButtons[down] == GAMEPAD_HOLD) ? hold : oldAction.down;
    oldAction.rotateLeft = (Gamepad::eventButtons[up] == GAMEPAD_HOLD) ? hold : oldAction.rotateLeft;
    oldAction.drop = (Gamepad::eventButtons[x] == GAMEPAD_HOLD) ? hold : oldAction.drop;
    oldAction.pageUp = (Gamepad::eventButtons[right_bumper] == GAMEPAD_HOLD) ? hold : oldAction.pageUp;
    oldAction.pageDown = (Gamepad::eventButtons[left_bumper] == GAMEPAD_HOLD) ? hold : oldAction.pageDown;
    oldAction.enter = (Gamepad::eventButtons[start] == GAMEPAD_HOLD) ? hold : oldAction.enter;
    oldAction.esc = (Gamepad::eventButtons[back] == GAMEPAD_HOLD) ? hold : oldAction.esc;

    for (int & button : Gamepad::buttons) {
        button = GAMEPAD_RELEASE;
    }
}