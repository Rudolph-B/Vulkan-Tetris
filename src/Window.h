/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#ifndef VULKAN_TETRIS_WINDOW_H
#define VULKAN_TETRIS_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "constants.h"


class Window {
public:
    Window();
    ~Window();

    GLFWwindow* glfwWindow;

    bool shouldClose() const;

private:

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

};

#endif //VULKAN_TETRIS_WINDOW_H
