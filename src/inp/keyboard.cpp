//
// Created by Rudolph on 2023/08/01.
//
#include "keyboard.h"

int Keyboard::keys[KEYBOARD_N_KEYS] = {0};
int Keyboard::eventKeys[KEYBOARD_N_KEYS] = {0};

void Keyboard::keyCallback(GLFWwindow *window, int rawKey, int scancode, int action, int mods) {
    // Log
#ifndef NDEBUG
    std::cout << "Key: " << rawKey << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << std::endl;
#endif
    Keys key = getKeyFromRawKey(rawKey);

    if (action == GLFW_PRESS) {
        Keyboard::keys[key] = KEYBOARD_PRESS;
        Keyboard::eventKeys[key] = KEYBOARD_PRESS;
    }
    else if (action == GLFW_RELEASE) {
        Keyboard::eventKeys[key] = KEYBOARD_RELEASE;
    }
}

Keyboard::Keys Keyboard::getKeyFromRawKey(const int rawKey)  {
    switch (rawKey) {
        case GLFW_KEY_SPACE:
            return space;
        case GLFW_KEY_APOSTROPHE:
            return apostrophe;
        case GLFW_KEY_COMMA:
            return comma;
        case GLFW_KEY_MINUS:
            return minus;
        case GLFW_KEY_PERIOD:
            return period;
        case GLFW_KEY_SLASH:
            return slash;
        case GLFW_KEY_0:
            return n0;
        case GLFW_KEY_1:
            return n1;
        case GLFW_KEY_2:
            return n2;
        case GLFW_KEY_3:
            return n3;
        case GLFW_KEY_4:
            return n4;
        case GLFW_KEY_5:
            return n5;
        case GLFW_KEY_6:
            return n6;
        case GLFW_KEY_7:
            return n7;
        case GLFW_KEY_8:
            return n8;
        case GLFW_KEY_9:
            return n9;
        case GLFW_KEY_SEMICOLON:
            return semicolon;
        case GLFW_KEY_EQUAL:
            return equal;
        case GLFW_KEY_A:
            return a;
        case GLFW_KEY_B:
            return b;
        case GLFW_KEY_C:
            return c;
        case GLFW_KEY_D:
            return d;
        case GLFW_KEY_E:
            return e;
        case GLFW_KEY_F:
            return f;
        case GLFW_KEY_G:
            return g;
        case GLFW_KEY_H:
            return h;
        case GLFW_KEY_I:
            return i;
        case GLFW_KEY_J:
            return j;
        case GLFW_KEY_K:
            return k;
        case GLFW_KEY_L:
            return l;
        case GLFW_KEY_M:
            return m;
        case GLFW_KEY_N:
            return n;
        case GLFW_KEY_O:
            return o;
        case GLFW_KEY_P:
            return p;
        case GLFW_KEY_Q:
            return q;
        case GLFW_KEY_R:
            return r;
        case GLFW_KEY_S:
            return s;
        case GLFW_KEY_T:
            return t;
        case GLFW_KEY_U:
            return u;
        case GLFW_KEY_V:
            return v;
        case GLFW_KEY_W:
            return w;
        case GLFW_KEY_X:
            return x;
        case GLFW_KEY_Y:
            return y;
        case GLFW_KEY_Z:
            return z;
        case GLFW_KEY_LEFT_BRACKET:
            return leftBracket;
        case GLFW_KEY_BACKSLASH:
            return backslash;
        case GLFW_KEY_RIGHT_BRACKET:
            return rightBracket;
        case GLFW_KEY_GRAVE_ACCENT:
            return graveAccent;
        case GLFW_KEY_WORLD_1:
            return world1;
        case GLFW_KEY_WORLD_2:
            return world2;
        case GLFW_KEY_ESCAPE:
            return escape;
        case GLFW_KEY_ENTER:
            return enter;
        case GLFW_KEY_TAB:
            return tab;
        case GLFW_KEY_BACKSPACE:
            return backspace;
        case GLFW_KEY_INSERT:
            return insert;
        case GLFW_KEY_DELETE:
            return del;
        case GLFW_KEY_RIGHT:
            return right;
        case GLFW_KEY_LEFT:
            return left;
        case GLFW_KEY_DOWN:
            return down;
        case GLFW_KEY_UP:
            return up;
        case GLFW_KEY_PAGE_UP:
            return pageUp;
        case GLFW_KEY_PAGE_DOWN:
            return pageDown;
        case GLFW_KEY_HOME:
            return home;
        case GLFW_KEY_END:
            return end;
        case GLFW_KEY_CAPS_LOCK:
            return capsLock;
        case GLFW_KEY_SCROLL_LOCK:
            return scrollLock;
        case GLFW_KEY_NUM_LOCK:
            return numLock;
        case GLFW_KEY_PRINT_SCREEN:
            return printScreen;
        case GLFW_KEY_PAUSE:
            return pause;
        case GLFW_KEY_F1:
            return f1;
        case GLFW_KEY_F2:
            return f2;
        case GLFW_KEY_F3:
            return f3;
        case GLFW_KEY_F4:
            return f4;
        case GLFW_KEY_F5:
            return f5;
        case GLFW_KEY_F6:
            return f6;
        case GLFW_KEY_F7:
            return f7;
        case GLFW_KEY_F8:
            return f8;
        case GLFW_KEY_F9:
            return f9;
        case GLFW_KEY_F10:
            return f10;
        case GLFW_KEY_F11:
            return f11;
        case GLFW_KEY_F12:
            return f12;
        case GLFW_KEY_F13:
            return f13;
        case GLFW_KEY_F14:
            return f14;
        case GLFW_KEY_F15:
            return f15;
        case GLFW_KEY_F16:
            return f16;
        case GLFW_KEY_F17:
            return f17;
        case GLFW_KEY_F18:
            return f18;
        case GLFW_KEY_F19:
            return f19;
        case GLFW_KEY_F20:
            return f20;
        case GLFW_KEY_F21:
            return f21;
        case GLFW_KEY_F22:
            return f22;
        case GLFW_KEY_F23:
            return f23;
        case GLFW_KEY_F24:
            return f24;
        case GLFW_KEY_F25:
            return f25;
        case GLFW_KEY_KP_0:
            return kp0;
        case GLFW_KEY_KP_1:
            return kp1;
        case GLFW_KEY_KP_2:
            return kp2;
        case GLFW_KEY_KP_3:
            return kp3;
        case GLFW_KEY_KP_4:
            return kp4;
        case GLFW_KEY_KP_5:
            return kp5;
        case GLFW_KEY_KP_6:
            return kp6;
        case GLFW_KEY_KP_7:
            return kp7;
        case GLFW_KEY_KP_8:
            return kp8;
        case GLFW_KEY_KP_9:
            return kp9;
        case GLFW_KEY_KP_DECIMAL:
            return kpDecimal;
        case GLFW_KEY_KP_DIVIDE:
            return kpDivide;
        case GLFW_KEY_KP_MULTIPLY:
            return kpMultiply;
        case GLFW_KEY_KP_SUBTRACT:
            return kpSubtract;
        case GLFW_KEY_KP_ADD:
            return kpAdd;
        case GLFW_KEY_KP_ENTER:
            return kpEnter;
        case GLFW_KEY_KP_EQUAL:
            return kpEqual;
        case GLFW_KEY_LEFT_SHIFT:
            return leftShift;
        case GLFW_KEY_LEFT_CONTROL:
            return leftControl;
        case GLFW_KEY_LEFT_ALT:
            return leftAlt;
        case GLFW_KEY_LEFT_SUPER:
            return leftSuper;
        case GLFW_KEY_RIGHT_SHIFT:
            return rightShift;
        case GLFW_KEY_RIGHT_CONTROL:
            return rightControl;
        case GLFW_KEY_RIGHT_ALT:
            return rightAlt;
        case GLFW_KEY_RIGHT_SUPER:
            return rightSuper;
        case GLFW_KEY_MENU:
            return menu;
        default:
            return unknown;
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

void Keyboard::updateAction(Action &oldAction) {
    oldAction.left = (Keyboard::keys[left] == KEYBOARD_PRESS) ? press : oldAction.left;
    oldAction.right = (Keyboard::keys[right] == KEYBOARD_PRESS) ? press : oldAction.right;
    oldAction.down = (Keyboard::keys[down] == KEYBOARD_PRESS) ? press : oldAction.down;
    oldAction.rotateLeft = (Keyboard::keys[up] == KEYBOARD_PRESS) ? press : oldAction.rotateLeft;
    oldAction.drop = (Keyboard::keys[space] == KEYBOARD_PRESS) ? press : oldAction.drop;
    oldAction.pageUp = (Keyboard::keys[pageUp] == KEYBOARD_PRESS) ? press : oldAction.pageUp;
    oldAction.pageDown = (Keyboard::keys[pageDown] == KEYBOARD_PRESS) ? press : oldAction.pageDown;
    oldAction.enter = (Keyboard::keys[enter] == KEYBOARD_PRESS) ? press : oldAction.enter;
    oldAction.esc = (Keyboard::keys[escape] == KEYBOARD_PRESS) ? press : oldAction.esc;
    oldAction.n1 = (Keyboard::keys[n1] == KEYBOARD_PRESS) ? press : oldAction.n1;
    oldAction.n2 = (Keyboard::keys[n2] == KEYBOARD_PRESS) ? press : oldAction.n2;

    oldAction.left = (Keyboard::eventKeys[left] == KEYBOARD_HOLD) ? hold : oldAction.left;
    oldAction.right = (Keyboard::eventKeys[right] == KEYBOARD_HOLD) ? hold : oldAction.right;
    oldAction.down = (Keyboard::eventKeys[down] == KEYBOARD_HOLD) ? hold : oldAction.down;
    oldAction.rotateLeft = (Keyboard::eventKeys[up] == KEYBOARD_HOLD) ? hold : oldAction.rotateLeft;
    oldAction.drop = (Keyboard::eventKeys[space] == KEYBOARD_HOLD) ? hold : oldAction.drop;
    oldAction.pageUp = (Keyboard::eventKeys[pageUp] == KEYBOARD_HOLD) ? hold : oldAction.pageUp;
    oldAction.pageDown = (Keyboard::eventKeys[pageDown] == KEYBOARD_HOLD) ? hold : oldAction.pageDown;
    oldAction.enter = (Keyboard::eventKeys[enter] == KEYBOARD_HOLD) ? hold : oldAction.enter;
    oldAction.esc = (Keyboard::eventKeys[escape] == KEYBOARD_HOLD) ? hold : oldAction.esc;
    oldAction.n1 = (Keyboard::eventKeys[n1] == KEYBOARD_HOLD) ? hold : oldAction.n1;
    oldAction.n2 = (Keyboard::eventKeys[n2] == KEYBOARD_HOLD) ? hold : oldAction.n2;

    for (int & key : Keyboard::keys) {
        key = KEYBOARD_RELEASE;
    }
}
