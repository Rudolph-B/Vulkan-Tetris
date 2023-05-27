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
        auto fpsStartTime = std::chrono::high_resolution_clock::now();
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

            /* CHECK FOR WINDOW UPDATES */
            glfwPollEvents();

            //<editor-fold desc="/* FORCE FRAME/TICK RATE */" defaultstate="collapsed">
            if (frameTime >= targetFrameTime) {
                /* PROCESS TETRIS TICK */
                if (vTetris.tick()) {
                    /* IF TETRIS STATE CHANGED */

                    /* UPDATE VERTEXES */
                    tEngine.updateVertices(vTetris.getVertices());
                }

                /* RENDER FRAME */
                tEngine.drawFrame();
                nDrawnFrames++;

                /* RESET FRAME TARGET */
                frameTime = 0;
            }
            //</editor-fold>

            //<editor-fold desc="/* PRINT FPS */" defaultstate="collapsed">
            float time = std::chrono::duration<float, std::chrono::seconds::period>(endTime - fpsStartTime).count();
            if (time >= 1.0) {
                std::cerr << "FPS: " << nDrawnFrames << std::endl;
                fpsStartTime = std::chrono::high_resolution_clock::now();
                nDrawnFrames = 0;
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
