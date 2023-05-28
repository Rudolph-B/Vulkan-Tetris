/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#include "window.h"
#include "engine.h"

Window::Window() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfwWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan-Tetris", nullptr, nullptr);
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, framebufferResizeCallback);
}

void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto tEngine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    tEngine->framebufferResized = true;
}

Window::~Window() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

bool Window::shouldClose() const {
    if (keyboard.esc) {
        return true;
    }

    return glfwWindowShouldClose(glfwWindow);
}

void Window::pollEvents() {
    /* CHECK FOR WINDOW UPDATES */
    glfwPollEvents();

    for (auto & key : KEYS) {
        if (glfwGetKey(glfwWindow, key) == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_LEFT:
                    keyboard.left = true;
                    break;
                case GLFW_KEY_RIGHT:
                    keyboard.right = true;
                    break;
                case GLFW_KEY_DOWN:
                    keyboard.down = true;
                    break;
                case GLFW_KEY_UP:
                    keyboard.up = true;
                    break;
                case GLFW_KEY_SPACE:
                    keyboard.space = true;
                    break;
                case GLFW_KEY_PAGE_UP:
                    keyboard.pageUp = true;
                    break;
                case GLFW_KEY_PAGE_DOWN:
                    keyboard.pageDown = true;
                    break;
                case GLFW_KEY_ENTER:
                    keyboard.enter = true;
                    break;
                case GLFW_KEY_ESCAPE:
                    keyboard.esc = true;
                    break;
                case GLFW_KEY_1:
                    keyboard.n1 = true;
                    break;
                case GLFW_KEY_2:
                    keyboard.n2 = true;
                    break;
                case GLFW_KEY_3:
                    keyboard.n3 = true;
                    break;
                case GLFW_KEY_4:
                    keyboard.n4 = true;
                    break;
                default:
                    break;
            }
        }
    }
}

Keyboard Window::getKeyboard() {
    return keyboard;
}

void Window::clearEvents() {
    keyboard = {GLFW_RELEASE};
}
