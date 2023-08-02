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
#define KEYBOARD_HOLD                   10
#define KEYBOARD_N_KEYS                 121

class Keyboard {
    enum Keys {
        unknown = 0,
        space = 1,
        apostrophe,
        comma,
        minus,
        period,
        slash,
        n0,
        n1,
        n2,
        n3,
        n4,
        n5,
        n6,
        n7,
        n8,
        n9,
        semicolon,
        equal,
        a,
        b,
        c,
        d,
        e,
        f,
        g,
        h,
        i,
        j,
        k,
        l,
        m,
        n,
        o,
        p,
        q,
        r,
        s,
        t,
        u,
        v,
        w,
        x,
        y,
        z,
        leftBracket,
        backslash,
        rightBracket,
        graveAccent,
        world1,
        world2,
        escape,
        enter,
        tab,
        backspace,
        insert,
        del,
        right,
        left,
        down,
        up,
        pageUp,
        pageDown,
        home,
        end,
        capsLock,
        scrollLock,
        numLock,
        printScreen,
        pause,
        f1,
        f2,
        f3,
        f4,
        f5,
        f6,
        f7,
        f8,
        f9,
        f10,
        f11,
        f12,
        f13,
        f14,
        f15,
        f16,
        f17,
        f18,
        f19,
        f20,
        f21,
        f22,
        f23,
        f24,
        f25,
        kp0,
        kp1,
        kp2,
        kp3,
        kp4,
        kp5,
        kp6,
        kp7,
        kp8,
        kp9,
        kpDecimal,
        kpDivide,
        kpMultiply,
        kpSubtract,
        kpAdd,
        kpEnter,
        kpEqual,
        leftShift,
        leftControl,
        leftAlt,
        leftSuper,
        rightShift,
        rightControl,
        rightAlt,
        rightSuper,
        menu,
    };
private:
    static int keys[KEYBOARD_N_KEYS];
    static int eventKeys[KEYBOARD_N_KEYS];
    static Keys getKeyFromRawKey(int rawKey);

public:
    static void keyCallback(GLFWwindow *window, int rawKey, int scancode, int action, int mods);
    static Action getAction();
    static void tick();

};

#endif //VULKAN_TETRIS_KEYBOARD_H
