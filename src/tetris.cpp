/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#include <iostream>
#include "tetris.h"

bool Tetris::tick(KeyBoard keyboard) {
    ticks++;

    if ((ticks % moveTickDivider) != 1) {

    }

    if ((ticks % dropTickDivider) != 1) {
        return false;
    }

    if (!canGoDown()) {
        placeTetrimino();
        newTetrimino();

        // Check cleared rows
        clearRows();

        // Check for game over
        if (!canGoDown()) {
            clearBoard();
            return false;
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

    for (int i = 0; i < 4; i++) {
        std::vector<Vertex> newVertices = buildVertices(cursor_x + TETRIMINO_SHAPES[cursor_tetrimino][i][0], cursor_y + TETRIMINO_SHAPES[cursor_tetrimino][i][1], cursor_colour);
        vertices.insert(std::end(vertices), std::begin(newVertices), std::end(newVertices));
    }

    return vertices;
}

Tetris::Tetris() {
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
    for (int i = 0; i < 4; i++) {
        uint8_t x = cursor_x + TETRIMINO_SHAPES[cursor_tetrimino][i][0];
        uint8_t y = cursor_y + TETRIMINO_SHAPES[cursor_tetrimino][i][1];
        board[x][y] = cursor_colour;
    }

    score += 10;
}

bool Tetris::canGoDown() {
    return canGoHere(0, -1);
}

bool Tetris::canGoHere(int8_t x, int8_t y) {
    for (int i = 0; i < 4; i++) {
        int8_t bx = cursor_x + TETRIMINO_SHAPES[cursor_tetrimino][i][0] + x;
        int8_t by = cursor_y + TETRIMINO_SHAPES[cursor_tetrimino][i][1] + y;
        if (by < 0 | bx < 0 | bx >= M_WIDTH) {
            return false;
        }
        if (board[bx][by]) {
            return false;
        }
    }
    return true;
}

void Tetris::newTetrimino() {
    cursor_x = M_WIDTH / 2;
    cursor_tetrimino = randomTetrimino();
    cursor_colour = randomColour();
    cursor_y = M_HEIGHT - 1;
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