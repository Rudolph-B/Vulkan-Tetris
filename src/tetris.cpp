/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#include <iostream>
#include "tetris.h"

bool Tetris::tick(KeyBoard keyboard) {
    ticks++;
    bool state_changed = false;

    if (keyboard.enter) {
        clearBoard();
        start = true;
    }

    if (!start) {
        return false;
    }

    if (keyboard.space && nextSpaceTick < ticks) {
        while (canGoDown()) {
            cursor_y--;
        }

        nextLeftTick = ticks + moveTickOffset * 2;
        return true;
    }

    if (keyboard.left && nextLeftTick < ticks) {
        if (canGoLeft()) {
            cursor_x--;
            nextLeftTick = ticks + moveTickOffset;
            state_changed = true;
        }
    }

    if (keyboard.right && nextRightTick < ticks) {
        if (canGoRight()) {
            cursor_x++;
            nextRightTick = ticks + moveTickOffset;
            state_changed = true;
        }
    }

    if (keyboard.up && nextUpTick < ticks) {
        rotateLeft();
        nextUpTick = ticks + moveTickOffset * 3;
        state_changed = true;
    }

    if (keyboard.down && nextDownTick < ticks) {
        if (canGoDown()) {
            nextDownTick = ticks + moveTickOffset;
            cursor_y--;
            state_changed = true;
        }
    }

    if (((ticks % dropTickDivider) != 1)) {
        return state_changed;
    }

    if (!canGoDown()) {
        placeTetrimino();
        newTetrimino();

        // Check cleared rows
        clearRows();

        // Check for game over
        if (!canGoDown()) {
            start = false;
            return true;
        }
    }
    cursor_y--;

    return true;
}

std::vector<Vertex> Tetris::getVertices() {
    std::vector<Vertex> vertices;

    /* ITERATE OVER BOARD */
    for (int i = 0; i < M_WIDTH; i++) {
        for (int j = 0; j < M_HEIGHT; j++) {
            /* ADD VERTICES IF BLOCK IS NOT EMPTY */
            if (board[i][j]) {
                //<editor-fold desc="/* PUSH VERTICES FOR EACH CORNER OF THE BLOCK */" defaultstate="collapsed">
                std::vector<Vertex> newVertices = buildVertices(i, j, board[i][j]);
                vertices.insert(std::end(vertices), std::begin(newVertices), std::end(newVertices));

                //</editor-fold>
            }
        }
    }

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if ((*TETRIMINO_SHAPES[cursor_tetrimino])[cursor_rotation][x][y]) {
                std::vector<Vertex> newVertices = buildVertices(cursor_x + x, cursor_y + y, cursor_colour);
                vertices.insert(std::end(vertices), std::begin(newVertices), std::end(newVertices));
            }
        }
    }

    return vertices;
}

Tetris::Tetris() {
    ticks = 0;
    ticks = 0;
    srand(time(nullptr));

    newTetrimino();
    clearBoard();
}

Tetris::~Tetris() {

}

std::vector<Vertex> Tetris::buildVertices(int i, int j, Colour c) {
    return {
        {{(float)i / (float)M_WIDTH, (float)j / (float)M_HEIGHT},{0, 0}, c},
        {{(float)(i + 1) / (float)M_WIDTH, (float)j / (float)M_HEIGHT},{0, 0}, c},
        {{(float)i / (float)M_WIDTH, (float)(j + 1) / (float)M_HEIGHT},{0, 0}, c},
        {{(float)(i + 1) / (float)M_WIDTH, (float)(j + 1) / (float)M_HEIGHT},{0, 0}, c},
        {{(float)i / (float)M_WIDTH, (float)(j + 1) / (float)M_HEIGHT},{0, 0}, c},
        {{(float)(i + 1) / (float)M_WIDTH, (float)j / (float)M_HEIGHT},{0, 0}, c}
    };
}

void Tetris::placeTetrimino() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if ((*TETRIMINO_SHAPES[cursor_tetrimino])[cursor_rotation][x][y]) {
                int bx = cursor_x + x;
                int by = cursor_y + y;
                if (!(by < 0 | bx < 0 | bx >= M_WIDTH)) {
                    board[bx][by] = cursor_colour;
                }
            }
        }
    }

    score += 10;
}

void Tetris::rotateLeft() {
    if (canGoHere(0,0,1)) {
        cursor_rotation = (cursor_rotation + 1) % 4;
    }
}

bool Tetris::canGoDown() {
    return canGoHere(0, -1);
}

bool Tetris::canGoLeft() {
    return canGoHere(-1, 0);
}

bool Tetris::canGoRight() {
    return canGoHere(1, 0);
}

bool Tetris::canGoHere(int8_t ox, int8_t oy, int r) {
    int cr = (cursor_rotation + r) % 4;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if ((*TETRIMINO_SHAPES[cursor_tetrimino])[cr][x][y]) {
                int bx = cursor_x + x + ox;
                int by = cursor_y + y + oy;
                if (by < 0 | bx < 0 | bx >= M_WIDTH) {
                    return false;
                }
                if (board[bx][by]) {
                    return false;
                }
            }
        }
    }

    return true;
}

void Tetris::newTetrimino() {
    cursor_x = M_WIDTH / 2;
    cursor_tetrimino = randomTetrimino();
    cursor_colour = randomColour();
    cursor_y = M_HEIGHT - 1;
    cursor_rotation = 0;
}

void Tetris::clearRows() {
    uint8_t nClears = 0;
    for (int i = 0; i < M_HEIGHT; i++) {
        bool rowFull = true;
        for (auto & row : board) {
            if (!row[i]) {
                rowFull = false;
                break;
            }
        }
        if (rowFull) {
            for (auto & row : board) {
                row[i] = Colour::NONE;
            }
            for (int j = i; j < M_HEIGHT - 1; j++) {
                for (auto & k : board) {
                    k[j] = k[j + 1];
                }
            }
            i--;
            nClears++;
        }
    }

    switch (nClears) {
        case 1:
            score += 100;
            break;
        case 2:
            score += 300;
            break;
        case 3:
            score += 600;
            break;
        case 4:
            score += 1000;
            break;
        default :
            break;
    }
}

void Tetris::clearBoard() {
    score = 0;

    for (auto & row : board) {
        for (auto & cell : row) {
            cell = Colour::NONE;
        }
    }
}

Tetris::Colour Tetris::randomColour() {
    static std::vector<Colour> bag = {
            Colour::RED, Colour::GREEN, Colour::BLUE, Colour::YELLOW, Colour::ORANGE, Colour::PURPLE
    };

    int index = rand() % bag.size();
    Colour colour = bag[index];

    // Remove the chosen colour from the bag
    bag.erase(bag.begin() + index);

    // Refill the bag if it becomes empty
    if (bag.empty()) {
        bag = {
            Colour::RED, Colour::GREEN, Colour::BLUE, Colour::YELLOW, Colour::ORANGE, Colour::PURPLE
        };
    }

    return colour;
}

Tetris::Tetriminos Tetris::randomTetrimino() {
    static std::vector<Tetriminos> bag = {
            Tetriminos::I, Tetriminos::J, Tetriminos::L, Tetriminos::O, Tetriminos::S, Tetriminos::T, Tetriminos::Z
    };

    int index = rand() % bag.size();
    Tetriminos tetrimino = bag[index];

    // Remove the chosen colour from the bag
    bag.erase(bag.begin() + index);

    // Refill the bag if it becomes empty
    if (bag.empty()) {
        bag = {
            Tetriminos::I, Tetriminos::J, Tetriminos::L, Tetriminos::O, Tetriminos::S, Tetriminos::T, Tetriminos::Z
        };
    }

    return tetrimino;
}
