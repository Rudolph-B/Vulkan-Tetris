/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#include "window.h"
#include "engine.h"

/* WINDOW SIZE */
extern const int WIDTH;
extern const int HEIGHT;

Window::Window() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan-Tetris", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto tEngine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    tEngine->framebufferResized = true;
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
