/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#ifndef VULKAN_TETRIS_TETRIS_H
#define VULKAN_TETRIS_TETRIS_H

#include "../structs.h"
#include "../scene.h"

class Tetris: public Scene {
public:
    //<editor-fold desc="/* PUBLIC CONSTANTS */" defaultstate="collapsed">
    // Board dimensions
    static const int B_HEIGHT = 20;
    static const int B_WIDTH = 10;
    //</editor-fold>

    //<editor-fold desc="/* PUBLIC METHODS */" defaultstate="collapsed">
    Tetris();

    ~Tetris();

    Result tick(Action action) override;

    std::vector<Vertex> getVertices() override;
    //</editor-fold>

    //<editor-fold desc="/* PUBLIC VARIABLES */" defaultstate="collapsed">
    //</editor-fold>

    //<editor-fold desc="/* PUBLIC ENUMS */" defaultstate="collapsed">
    // Used to represent the colour of a tetrimino
    enum Colour {
        NONE = 0,
        RED = 1,
        GREEN = 2,
        BLUE = 3,
        YELLOW = 4,
        ORANGE = 5,
        PURPLE = 6
    };
    constexpr static const glm::vec3 Colours[8] = {
        {1.0, 1.0, 1.0},
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 1.0, 1.0},
        {1.0, 0.0, 1.0},
        {1.0, 1.0, 0.0},
        {1.0, 1.0, 1.0}
    };

    // Used to represent the type of tetrimino
    enum Tetriminos {
        I = 0,
        J = 1,
        L = 2,
        O = 3,
        S = 4,
        T = 5,
        Z = 6
    };
    //</editor-fold>

private:
    //<editor-fold desc="/* PRIVATE CONSTANTS */" defaultstate="collapsed">

    //<editor-fold desc="/* TETRIMINO ROTATIONS CONSTANTS */" defaultstate="collapsed">
    //<editor-fold desc="/* O_ROTATIONS */" defaultstate="collapsed">
    const int8_t O_ROTATIONS[4][4][4] = {
            {
                    {1, 1, 0, 0},
                    {1, 1, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {1, 1, 0, 0},
                    {1, 1, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {1, 1, 0, 0},
                    {1, 1, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {1, 1, 0, 0},
                    {1, 1, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}
            }
    };
    //</editor-fold>

    //<editor-fold desc="/* I_ROTATIONS */" defaultstate="collapsed">
    const int8_t I_ROTATIONS[4][4][4] = {
            {
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0}
            },
            {
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0}
            }
    };
    //</editor-fold>

    //<editor-fold desc="/* J_ROTATIONS */" defaultstate="collapsed">
    const int8_t J_ROTATIONS[4][4][4] = {
            {
                    {1, 0, 0, 0},
                    {1, 1, 1, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 1, 1, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 0, 0, 0},
                    {1, 1, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 0}
            },
            {
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {1, 1, 0, 0},
                    {0, 0, 0, 0}
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

    // Lookup table containing all possible tetrimino position
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
    static std::vector<Vertex> buildVertices(int i, int j, Colour c, int8_t age);

    void placeTetrimino();

    bool canGoDown();

    void newTetrimino();

    bool canGoHere(int8_t x, int8_t y, int r = 0);

    bool canGoLeft();

    bool canGoRight();

    void rotateLeft();

    void clearRows();

    void clearBoard();

    static Colour randomColour();

    static Tetriminos randomTetrimino();
    //</editor-fold>

    //<editor-fold desc="/* PRIVATE VARIABLES */" defaultstate="collapsed">
    // Default piece moves down every 0.5s (60 ticks per second / 30 ticks per drop)
    const int dropTickDivider[7] = {8, 12, 20, 30, 45, 80, 120};
    int iDropTickDivider = 3;

    // Use to delay certain user moves
    int moveTickOffset = 8;
    int nextLeftTick = 0;
    int nextRightTick = 0;
    int nextDownTick = 0;
    int nextUpTick = 0;
    int nextSpeedUpdateTick = 0;

    // Score
    int64_t score{};
    // Add drop to the top off the board to allow for tetriminos to spawn
    Colour board[B_WIDTH][B_HEIGHT + 4] = {Colour::NONE};
    bool started = false;

    // Current tetrimino
    int8_t cursorX{};
    int8_t cursorY{};
    Colour cursorColour;
    int cursorRotation{};
    Tetriminos cursorTetrimino;

    //</editor-fold>

    [[nodiscard]] int calculateNextMoveTick(int oldNextMoveTick) const;
};

#endif //VULKAN_TETRIS_TETRIS_H
