/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#include <iostream>
#include "tetris.h"

bool Tetris::tick() {
    ticks++;

    if (!(ticks % TICK_DIVIDER)) {
        return false;
    }

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
                // BL
                vertices.push_back({
                    .pos = {(float)i / (float)M_WIDTH, (float)j / (float)M_HEIGHT},
                    .tex = {0, 0},
                    .type = board[i][j]
                });

                // BR
                vertices.push_back({
                    .pos = {(float)(i + 1) / (float)M_WIDTH, (float)j / (float)M_HEIGHT},
                    .tex = {0, 0},
                    .type = board[i][j]
                });

                // TL
                vertices.push_back({
                    .pos = {(float)i / (float)M_WIDTH, (float)(j + 1) / (float)M_HEIGHT},
                    .tex = {0, 0},
                    .type = board[i][j]
                });

                // TL
                vertices.push_back({
                    .pos = {(float)(i + 1) / (float)M_WIDTH, (float)(j + 1) / (float)M_HEIGHT},
                    .tex = {0, 0},
                    .type = board[i][j]
                });

                // TR
                vertices.push_back({
                    .pos = {(float)i / (float)M_WIDTH, (float)(j + 1) / (float)M_HEIGHT},
                    .tex = {0, 0},
                    .type = board[i][j]
                });

                // BR
                vertices.push_back({
                    .pos = {(float)(i + 1) / (float)M_WIDTH, (float)j / (float)M_HEIGHT},
                    .tex = {0, 0},
                    .type = board[i][j]
                });

                //</editor-fold>
            }
        }
    }

    return vertices;
}

Tetris::Tetris() {
    board[0][0] = BLUE;
    board[0][1] = BLUE;
    board[1][0] = BLUE;
    board[2][0] = BLUE;

    board[2][1] = RED;
    board[3][1] = RED;
    board[3][0] = RED;
    board[4][0] = RED;

    board[3][15] = GREEN;
    board[3][14] = GREEN;
    board[3][13] = GREEN;
    board[3][12] = GREEN;
}

Tetris::~Tetris() {

}
