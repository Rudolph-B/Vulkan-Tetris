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
#include "src/tetris.h"
//</editor-fold>

int main() {
    /* INIT */
    Window gWindow{};
    Engine tEngine{&gWindow};
    Tetris vTetris{};

    try {
        /* INIT STATS */
        auto startTime = std::chrono::high_resolution_clock::now();
        float frameTime = 0.0f; // Time since last frame
        const float targetFrameTime = 1.0f / 60.0f; // 60 FPS
        int nDrawnFrames = 0;

        /* MAIN LOOP */
        while (!gWindow.shouldClose()) {
            /* UPDATE FRAME TIME */
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> frameDuration = endTime - startTime;
            frameTime += frameDuration.count();
            startTime = endTime;


            /* PROCESS INPUT */
            gWindow.pollEvents();

            //<editor-fold desc="/* FORCE FRAME/TICK RATE */" defaultstate="collapsed">
            if (frameTime >= targetFrameTime) {
                Keyboard keyboard = gWindow.getKeyboard();

                /* PROCESS TETRIS TICK */
                if (vTetris.tick(keyboard)) {
                    /* IF TETRIS STATE CHANGED */

                    /* UPDATE VERTEXES */
                    tEngine.updateVertices(vTetris.getVertices());
                }

                /* RENDER FRAME */
                tEngine.updateSettings(keyboard);
                tEngine.drawFrame();
                nDrawnFrames++;
                frameTime = 0;

                /* CLEAR EVENTS */
                gWindow.clearEvents();
            }
            //</editor-fold>
        }

        tEngine.waitDeviceIdle();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
