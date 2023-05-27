/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

//<editor-fold desc="/* STANDARD INCLUDES */" defaultstate="collapsed">
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <optional>
//</editor-fold>

//<editor-fold desc="/* SOURCE INCLUDES */" defaultstate="collapsed">
#include "src/engine.h"
#include "src/constants.h"
#include "src/window.h"
//</editor-fold>

int main() {
    Window gWindow{};
    Engine tEngine{&gWindow};

    try {
        gWindow.shouldClose();
        while (!gWindow.shouldClose()) {
            glfwPollEvents();
            tEngine.drawFrame();
        }

        tEngine.waitDeviceIdle();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
