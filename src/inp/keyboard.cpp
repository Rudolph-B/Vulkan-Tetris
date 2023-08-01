//
// Created by Rudolph on 2023/08/01.
//
#include "Keyboard.h"

int Keyboard::keys[KEYBOARD_N_KEYS] = {0};
int Keyboard::eventKeys[KEYBOARD_N_KEYS] = {0};

void Keyboard::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Log
    std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << std::endl;
    if (action == GLFW_PRESS) {
        Keyboard::keys[key] += KEYBOARD_PRESS;
        Keyboard::eventKeys[key] = KEYBOARD_PRESS;
    }
    else if (action == GLFW_RELEASE) {
        Keyboard::eventKeys[key] = KEYBOARD_RELEASE;
    }
}

void Keyboard::tick() {
    // Loop through all keys
    for (int i = 0; i < KEYBOARD_N_KEYS; i++) {
        Keyboard::keys[i] = KEYBOARD_RELEASE;
        if (Keyboard::eventKeys[i] >= KEYBOARD_PRESS && Keyboard::eventKeys[i] < KEYBOARD_HOLD) {
            Keyboard::eventKeys[i] += KEYBOARD_PRESS;
        }
    }
}

Action Keyboard::getAction() {
    Action action = {release};

    // Loop through all keys
    for (int i = 0; i < KEYBOARD_N_KEYS; i++) {
        if (Keyboard::keys[i] == KEYBOARD_PRESS) {
            switch (i) {
                case GLFW_KEY_LEFT:
                    action.left = press;
                    break;
                case GLFW_KEY_RIGHT:
                    action.right = press;
                    break;
                case GLFW_KEY_DOWN:
                    action.down = press;
                    break;
                case GLFW_KEY_UP:
                    action.rotate_left = press;
                    break;
                case GLFW_KEY_SPACE:
                    action.drop = press;
                    break;
                case GLFW_KEY_PAGE_UP:
                    action.pageUp = press;
                    break;
                case GLFW_KEY_PAGE_DOWN:
                    action.pageDown = press;
                    break;
                case GLFW_KEY_ENTER:
                    action.enter = press;
                    break;
                case GLFW_KEY_ESCAPE:
                    action.esc = press;
                    break;
                case GLFW_KEY_1:
                    action.n1 = press;
                    break;
                case GLFW_KEY_2:
                    action.n2 = press;
                    break;
                default:
                    break;
            }
        }
        if (Keyboard::eventKeys[i] == KEYBOARD_HOLD) {
            switch (i) {
                case GLFW_KEY_LEFT:
                    action.left = hold;
                    break;
                case GLFW_KEY_RIGHT:
                    action.right = hold;
                    break;
                case GLFW_KEY_DOWN:
                    action.down = hold;
                    break;
                case GLFW_KEY_UP:
                    action.rotate_left = hold;
                    break;
                case GLFW_KEY_SPACE:
                    action.drop = hold;
                    break;
                case GLFW_KEY_PAGE_UP:
                    action.pageUp = hold;
                    break;
                case GLFW_KEY_PAGE_DOWN:
                    action.pageDown = hold;
                    break;
                case GLFW_KEY_ENTER:
                    action.enter = hold;
                    break;
                case GLFW_KEY_ESCAPE:
                    action.esc = hold;
                    break;
                case GLFW_KEY_1:
                    action.n1 = hold;
                    break;
                case GLFW_KEY_2:
                    action.n2 = hold;
                    break;
                default:
                    break;
            }
        }

    }

    return action;
}
