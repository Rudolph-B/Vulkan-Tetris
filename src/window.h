/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#ifndef VULKAN_TETRIS_WINDOW_H
#define VULKAN_TETRIS_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "constants.h"
#include "structs.h"
#include "inp/keyboard.h"
#include "inp/gamepad.h"

class Window {
public:
    Window();
    ~Window();
    [[nodiscard]] bool shouldClose() const;
    static void pollEvents();

    GLFWwindow* glfwWindow;

private:
    const int KEYS[13] = {
        GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_SPACE,
        GLFW_KEY_PAGE_UP, GLFW_KEY_PAGE_DOWN, GLFW_KEY_ENTER, GLFW_KEY_ESCAPE,
        GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4,
    };

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

};

#endif //VULKAN_TETRIS_WINDOW_H
