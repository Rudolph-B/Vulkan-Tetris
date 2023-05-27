/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#include <iostream>
#include "tetris.h"

bool Tetris::tick() {
    ticks++;

    if ((ticks % tickDivider) != 1) {
        return false;
    }

    if (!canGoDown()) {
        placeTetrimino();
        newTetrimino();
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
    // Select random tetrimino
    score = 0;
    ticks = 0;
    newTetrimino();

    board[0][0] = Colour::BLUE;
    board[0][1] = Colour::BLUE;
    board[1][0] = Colour::BLUE;
    board[2][0] = Colour::BLUE;

    board[2][1] = Colour::RED;
    board[3][1] = Colour::RED;
    board[3][0] = Colour::RED;
    board[4][0] = Colour::RED;

    board[3][15] = Colour::GREEN;
    board[3][14] = Colour::GREEN;
    board[3][13] = Colour::GREEN;
    board[3][12] = Colour::GREEN;
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
}

bool Tetris::canGoDown() {
    for (int i = 0; i < 4; i++) {
        uint8_t x = cursor_x + TETRIMINO_SHAPES[cursor_tetrimino][i][0];
        uint8_t y = cursor_y + TETRIMINO_SHAPES[cursor_tetrimino][i][1] - 1;
        if (y < 0) {
            return false;
        }
        if (board[x][y]) {
            return false;
        }
    }
    return true;
}

void Tetris::newTetrimino() {
    cursor_x = M_WIDTH / 2;
    cursor_tetrimino = (Tetriminos)(rand() % 7);
    cursor_colour = (Colour)(rand() % 6 + 1);
    cursor_y = M_HEIGHT - 1;
}
