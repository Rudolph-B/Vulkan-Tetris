/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#include "window.h"
#include "engine.h"

Window::Window() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfwWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan-Tetris", nullptr, nullptr);
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, framebufferResizeCallback);
    glfwSetKeyCallback(glfwWindow, Keyboard::keyCallback);
    glfwSetJoystickCallback(Gamepad::gamepadCallback);
}

void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto tEngine = reinterpret_cast<Engine *>(glfwGetWindowUserPointer(window));
    tEngine->framebufferResized = true;
}

Window::~Window() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

bool Window::shouldClose(Action action) const {
    /* CHECK FOR WINDOW CLOSE */
    if (action.esc == press) {
        return true;
    }

    return glfwWindowShouldClose(glfwWindow);
}

/**
 * Polls for events and updates the Keyboard struct.
 */
void Window::pollEvents() {
    /* CHECK FOR WINDOW UPDATES */
    glfwPollEvents();
}
