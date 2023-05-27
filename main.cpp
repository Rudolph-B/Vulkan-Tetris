/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

//<editor-fold desc="/* STANDARD INCLUDES */" defaultstate="collapsed">
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <optional>
//</editor-fold>

//<editor-fold desc="/* GLOBAL CONSTANTS */" defaultstate="collapsed">
/* WINDOW SIZE */
const int WIDTH = 800;
const int HEIGHT = 600;

/* MODEL AND TEXTURE PATHS */
const std::string MODEL_PATH = "mdl/temp.obj";
const std::string TEXTURE_PATH = "tex/temp.png";

const int MAX_FRAMES_IN_FLIGHT = 2;

/* ENABLES/DISABLES VALIDATION LAYERS*/
#ifdef NDEBUG
const bool E_VALIDATION_LAYERS = false;
#else
const bool E_VALIDATION_LAYERS = true;
#endif
//</editor-fold>

//<editor-fold desc="/* SOURCE INCLUDES */" defaultstate="collapsed">
#include "src/engine.h"
//</editor-fold>

int main() {
    Engine app{};

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
