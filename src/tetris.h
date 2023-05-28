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
    //<editor-fold desc="/* TETRIMINO ROTATIONS */" defaultstate="collapsed">
    //<editor-fold desc="/* O_ROTATIONS */" defaultstate="collapsed">
    const int8_t O_ROTATIONS[4][4][4] = {
            {
                    {1, 1, 0, 0},
                    {1, 1, 0, 0},
                    {0,  0, 0,0},
                    {0,  0, 0,0}
            },
            {
                    {1, 1, 0, 0},
                    {1, 1, 0, 0},
                    {0,  0, 0,0},
                    {0,  0, 0,0}
            },
            {
                    {1, 1, 0, 0},
                    {1, 1, 0, 0},
                    {0,  0, 0,0},
                    {0,  0, 0,0}
            },
            {
                    {1, 1, 0, 0},
                    {1, 1, 0, 0},
                    {0,  0, 0,0},
                    {0,  0, 0,0}
            }
    };
    //</editor-fold>

    //<editor-fold desc="/* I_ROTATIONS */" defaultstate="collapsed">
    const int8_t I_ROTATIONS[4][4][4] = {
            {
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0,  0, 0,0},
                    {0,  0, 0,0}
            },
            {
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0,  1, 0,0},
                    {0,  1, 0,0}
            },
            {
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0,  0, 0,0},
                    {0,  0, 0,0}
            },
            {
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0,  1, 0,0},
                    {0,  1, 0,0}
            }
    };
    //</editor-fold>

    //<editor-fold desc="/* J_ROTATIONS */" defaultstate="collapsed">
    const int8_t J_ROTATIONS[4][4][4] = {
            {
                    {1, 0, 0, 0},
                    {1, 1, 1, 0},
                    {0,  0, 0,0},
                    {0,  0, 0,0}
            },
            {
                    {0, 1, 1, 0},
                    {0, 1, 0, 0},
                    {0,  1, 0,0},
                    {0,  0, 0,0}
            },
            {
                    {0, 0, 0, 0},
                    {1, 1, 1, 0},
                    {0,  0, 1,0},
                    {0,  0, 0,0}
            },
            {
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {1,  1, 0,0},
                    {0,  0, 0,0}
            }
    };
    //</editor-fold>

    //<editor-fold desc="/* L_ROTATIONS */" defaultstate="collapsed">
    const int8_t L_ROTATIONS[4][4][4] = {
            {
                    {0, 0, 1, 0},
                    {1, 1, 1, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 1, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 0, 0, 0},
                    {1, 1, 1, 0},
                    {1, 0, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {1, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 0, 0}
            }
    };
    //</editor-fold>

    //<editor-fold desc="/* S_ROTATIONS */" defaultstate="collapsed">
    const int8_t S_ROTATIONS[4][4][4] = {
            {
                    {0, 1, 1, 0},
                    {1, 1, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 1, 0, 0},
                    {0, 1, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 0, 0, 0},
                    {0, 1, 1, 0},
                    {1, 1, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {1, 0, 0, 0},
                    {1, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 0, 0}
            }
    };
    //</editor-fold>

    //<editor-fold desc="/* T_ROTATIONS */" defaultstate="collapsed">
    const int8_t T_ROTATIONS[4][4][4] = {
            {
                    {0, 1, 0, 0},
                    {1, 1, 1, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 1, 0, 0},
                    {0, 1, 1, 0},
                    {0, 1, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 0, 0, 0},
                    {1, 1, 1, 0},
                    {0, 1, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 1, 0, 0},
                    {1, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 0, 0}
            }
    };
    //</editor-fold>

    //<editor-fold desc="/* J_ROTATIONS */" defaultstate="collapsed">
    const int8_t Z_ROTATIONS[4][4][4] = {
            {
                    {1, 1, 0, 0},
                    {0, 1, 1, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 0, 1, 0},
                    {0, 1, 1, 0},
                    {0, 1, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 0, 0, 0},
                    {1, 1, 0, 0},
                    {0, 1, 1, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 1, 0, 0},
                    {1, 1, 0, 0},
                    {1, 0, 0, 0},
                    {0, 0, 0, 0}
            }
    };
    //</editor-fold>

    //</editor-fold>
    const int8_t (*const TETRIMINO_SHAPES[7])[4][4][4] = {
        &I_ROTATIONS,
        &J_ROTATIONS,
        &L_ROTATIONS,
        &O_ROTATIONS,
        &S_ROTATIONS,
        &T_ROTATIONS,
        &Z_ROTATIONS
    };

    //</editor-fold>

    //<editor-fold desc="/* PRIVATE METHODS */" defaultstate="collapsed">
    static std::vector<Vertex> buildVertices(int i, int j, Colour c);

    void placeTetrimino();

    bool canGoDown();

    void newTetrimino();

    bool canGoHere(int8_t x, int8_t y, int r = 0);

    void clearRows();

    void clearBoard();
    //</editor-fold>

    //<editor-fold desc="/* PRIVATE VARIABLES */" defaultstate="collapsed">
    int dropTickDivider = 30;
    int moveTickOffset = 3;
    Colour board[M_WIDTH][M_HEIGHT + 4] = {Colour::NONE};
    bool start = false;

    int64_t score;
    int64_t ticks;
    int8_t cursor_x;
    int8_t cursor_y;
    Colour cursor_colour;
    int cursor_rotation;
    Tetriminos cursor_tetrimino;

    int nextLeftTick = 0;
    int nextRightTick = 0;
    int nextDownTick = 0;
    int nextUpTick = 0;
    int nextSpaceTick = 0;

    //</editor-fold>

    static Colour randomColour();
    static Tetriminos randomTetrimino();

    bool canGoLeft();

    bool canGoRight();

    void rotateLeft();

    void rotateRight();
};

#endif //VULKAN_TETRIS_TETRIS_H
