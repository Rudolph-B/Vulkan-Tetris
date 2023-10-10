/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

//<editor-fold desc="/* STANDARD INCLUDES */" defaultstate="collapsed">
#include <iostream>
#include <cstdlib>
#include <optional>
//</editor-fold>

//<editor-fold desc="/* SOURCE INCLUDES */" defaultstate="collapsed">
#include "src/engine.h"
#include "src/window.h"
#include "src/sce/tetris.h"
#include "src/sce/intro.h"
//</editor-fold>

int main() {
    /* INIT */
    Window gWindow{};
    Engine tEngine{&gWindow};
    Scene* cScene = new Intro{};
    Scene* nScene;
    bool shouldClose = false;

    try {
        /* INIT STATS */
        auto startTime = std::chrono::high_resolution_clock::now();
        float frameTime = 0.0f; // Time since last frame
        const float targetFrameTime = 1.0f / 60.0f; // 60 FPS
        int nDrawnFrames = 0;
        Action action;

        /* MAIN LOOP */
        while (!gWindow.shouldClose() && !shouldClose) {
            /* UPDATE FRAME TIME */
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> frameDuration = endTime - startTime;
            frameTime += frameDuration.count();
            startTime = endTime;


            /* PROCESS INPUT */
            Window::pollEvents();

            //<editor-fold desc="/* FORCE FRAME/TICK RATE */" defaultstate="collapsed">
            if (frameTime >= targetFrameTime) {
                action = {release};
                Keyboard::updateAction(action);
                Gamepad::updateAction(action);

                /* PROCESS TETRIS TICK */
                switch (cScene->tick(action)) {
                    case Scene::Result::update:
                        /* UPDATE VERTEXES */
                        tEngine.updateVertices(cScene->getVertices());
                        break;
                    case Scene::Result::next:
                        /* CHANGE SCENE */
                        nScene = cScene->nextScene();
                        delete cScene;
                        cScene = nScene;
                        break;
                    case Scene::Result::close:
                        /* CLOSE GAME */
                        shouldClose = true;
                        break;

                    case Scene::Result::noop:
                    default:
                        /* NOOP */
                        break;
                }

                /* RENDER FRAME */
                tEngine.updateSettings(action);
                tEngine.drawFrame();
                nDrawnFrames++;
                frameTime = 0;

                /* CLEAN INPUT */
                Keyboard::tick();
                Gamepad::tick();
            }
            //</editor-fold>
        }

        tEngine.waitDeviceIdle();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        delete cScene;
        return EXIT_FAILURE;
    }

    delete cScene;
    return EXIT_SUCCESS;
}
