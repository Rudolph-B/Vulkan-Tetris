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

    bool tick(KeyBoard keyboard);

    std::vector<Vertex> getVertices();
    //</editor-fold>

    //<editor-fold desc="/* PUBLIC VARIABLES */" defaultstate="collapsed">
    //</editor-fold>

    enum Colour {
        NONE = 0,
        RED = 1,
        GREEN = 2,
        BLUE = 3,
        YELLOW = 4,
        ORANGE = 5,
        PURPLE = 6
    };

    enum Tetriminos {
        I = 0,
        J = 1,
        L = 2,
        O = 3,
        S = 4,
        T = 5,
        Z = 6
    };

private:
    //<editor-fold desc="/* PRIVATE CONSTANTS */" defaultstate="collapsed">
    const int8_t TETRIMINO_SHAPES[7][4][2] = {
    {{0, 0}, {0, 1}, {0,  2}, {0,  3}}, // I
    {{0, 0}, {0, 1}, {0,  2}, {1,  2}}, // J
    {{0, 0}, {0, 1}, {0,  2}, {-1, 2}}, // L
    {{0, 0}, {0, 1}, {1,  0}, {1,  1}}, // O
    {{0, 0}, {0, 1}, {1,  1}, {1,  2}}, // S
    {{0, 0}, {0, 1}, {0,  2}, {1,  1}}, // T
    {{0, 0}, {0, 1}, {-1, 1}, {-1, 2}} // Z
    };

    //</editor-fold>

    //<editor-fold desc="/* PRIVATE METHODS */" defaultstate="collapsed">
    static std::vector<Vertex> buildVertices(int i, int j, Colour c);

    void placeTetrimino();

    bool canGoDown();

    void newTetrimino();

    bool canGoHere(int8_t x, int8_t y);

    void clearRows();

    void clearBoard();
    //</editor-fold>

    //<editor-fold desc="/* PRIVATE VARIABLES */" defaultstate="collapsed">
    int dropTickDivider = 30;
    int moveTickDivider = 3;
    Colour board[M_WIDTH][M_HEIGHT + 2] = {Colour::NONE};

    int64_t score;
    int64_t ticks;
    int8_t cursor_x;
    int8_t cursor_y;
    Colour cursor_colour;
    Tetriminos cursor_tetrimino;
    //</editor-fold>

    static Colour randomColour();
    static Tetriminos randomTetrimino();
};


#endif //VULKAN_TETRIS_TETRIS_H
