/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#ifndef VULKAN_TETRIS_TETRIS_H
#define VULKAN_TETRIS_TETRIS_H

#include "structs.h"

class Tetris {
public:
    //<editor-fold desc="/* PUBLIC CONSTANTS */" defaultstate="collapsed">
    static const int M_HEIGHT = 20;
    static const int M_WIDTH = 10;
    //</editor-fold>

    //<editor-fold desc="/* PUBLIC METHODS */" defaultstate="collapsed">
    Tetris();
    ~Tetris();

    bool tick();
    std::vector<Vertex> getVertices();
    //</editor-fold>

    //<editor-fold desc="/* PUBLIC VARIABLES */" defaultstate="collapsed">
    //</editor-fold>

private:
    //<editor-fold desc="/* PRIVATE CONSTANTS */" defaultstate="collapsed">
    const int TICK_DIVIDER = 10;
    const int8_t BLUE = 1;
    const int8_t RED = 2;
    const int8_t GREEN = 3;
    //</editor-fold>

    //<editor-fold desc="/* PRIVATE METHODS */" defaultstate="collapsed">
    //</editor-fold>

    //<editor-fold desc="/* PRIVATE VARIABLES */" defaultstate="collapsed">
    int8_t board[M_WIDTH][M_HEIGHT] = {0};
    int64_t score = 0;
    int64_t ticks = 0;
    //</editor-fold>
};


#endif //VULKAN_TETRIS_TETRIS_H
