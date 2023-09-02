/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#include <iostream>
#include "tetris.h"

/**
 * Process a tetris tick
 *
 * @param action
 * @return
 */
bool Tetris::tick(Action action) {
    ticks++;
    bool state_changed = false;

    // Reset game on enter
    if (action.enter == press) {
        newTetrimino();
        clearBoard();
        start = true;
    }

    // Increase speed on page rotateLeft
    if (action.pageUp == press && nextSpeedUpdateTick < ticks) {
        iDropTickDivider = std::max(0, std::min(iDropTickDivider - 1, 6));
        nextSpeedUpdateTick = ticks + moveTickOffset * 3;
    }

    // Increase speed on page rotateLeft
    if (action.pageDown == press && nextSpeedUpdateTick < ticks) {
        iDropTickDivider = std::max(0, std::min(iDropTickDivider + 1, 6));
        nextSpeedUpdateTick = ticks + moveTickOffset * 3;
    }

    // Wait for enter to start
    if (!start) {
        return false;
    }

    //<editor-fold desc="/* USER MOVE TETRIMINO */" defaultstate="collapsed">
    if (action.left == press) {
        if (canGoLeft()) {
            cursorX--;
            state_changed = true;
        }
    }

    if (action.left == hold && nextLeftTick <= ticks) {
        if (canGoLeft()) {
            cursorX--;
            nextLeftTick = calculateNextMoveTick(nextLeftTick);
            state_changed = true;
        }
    }

    if (action.right == press) {
        if (canGoRight()) {
            cursorX++;
            state_changed = true;
        }
    }

    if (action.right == hold && nextRightTick <= ticks) {
        if (canGoRight()) {
            cursorX++;
            nextRightTick = calculateNextMoveTick(nextRightTick);
            state_changed = true;
        }
    }

    // Drop tetrimino on drop
    if (action.drop == press) {
        while (canGoDown()) {
            cursorY--;
        }
        state_changed = true;
    }

    // Rotate tetrimino on rotateLeft
    if (action.rotateLeft == press) {
        rotateLeft();
        state_changed = true;
    }

    // Rotate tetrimino on rotateLeft
    if (action.rotateLeft == hold && nextUpTick <= ticks) {
        rotateLeft();
        nextUpTick = calculateNextMoveTick(nextUpTick) + 4;
        state_changed = true;
    }

    // Move tetrimino down
    if (action.down == press) {
        if (canGoDown()) {
            cursorY--;
            state_changed = true;
        }
    }

    // Move tetrimino down
    if (action.down == hold && nextDownTick <= ticks) {
        if (canGoDown()) {
            nextDownTick = calculateNextMoveTick(nextDownTick);
            cursorY--;
            state_changed = true;
        }
    }
    //</editor-fold>

    // Check if the game should move the tetrimino down
    if (((ticks % dropTickDivider[iDropTickDivider]) != 1)) {
        return state_changed;
    }

    // Check if the tetrimino can move down
    if (!canGoDown()) {

        // Place tetrimino on board if not
        placeTetrimino();
        clearRows();

        // Initialise new tetrimino
        newTetrimino();

        // Check for game over
        if (!canGoDown()) {
            // Pause game
            start = false;
            return true;
        }
    }

    // Move tetrimino down
    cursorY--;
    return true;
}

/**
 * Checks for button holds and speeds rotateLeft movement if true
 *
 * @param ticks
 * @param oldNextMoveTick
 * @return Next tick to move tetrimino
 */
int Tetris::calculateNextMoveTick(int oldNextMoveTick) const {// Check for button hold
    if (oldNextMoveTick == ticks) {
        // Allow faster movement if held
        oldNextMoveTick = ticks + moveTickOffset / 3;
    } else {
        oldNextMoveTick = ticks + moveTickOffset;
    }

    return oldNextMoveTick;
}

/**
 * Get all vertices for the tetris game
 */
std::vector<Vertex> Tetris::getVertices() {
    std::vector<Vertex> vertices;

    /* ITERATE OVER BOARD */
    for (int i = 0; i < B_WIDTH; i++) {
        for (int j = 0; j < B_HEIGHT; j++) {
            /* ADD VERTICES IF BLOCK IS NOT EMPTY */
            if (board[i][j]) {

                /* PUSH VERTICES FOR EACH CORNER OF THE BLOCK */
                std::vector<Vertex> newVertices = buildVertices(i, j, board[i][j], 0);
                vertices.insert(std::end(vertices), std::begin(newVertices), std::end(newVertices));
            }
        }
    }

    /* ITERATE OVER TETRIMINO */
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {

            /* ADD VERTICES IF BLOCK IS NOT EMPTY */
            if ((*TETRIMINO_SHAPES[cursorTetrimino])[cursorRotation][x][y]) {
                std::vector<Vertex> newVertices = buildVertices(cursorX + x, cursorY + y, cursorColour, 1);
                vertices.insert(std::end(vertices), std::begin(newVertices), std::end(newVertices));
            }
        }
    }

    return vertices;
}

/**
 * @return
 */
Tetris::Tetris() {
    // Set random seed
    srand(time(nullptr));

    newTetrimino();
    clearBoard();
}

/**
 * @return
 */
Tetris::~Tetris() = default;

/**
 * Build vertices for a block
 *
 * @param i
 * @param j
 * @param c
 * @return
 */
std::vector<Vertex> Tetris::buildVertices(int i, int j, Colour c, int8_t age) {
    return {
            {{(float) i / (float) B_WIDTH,       (float) j / (float) B_HEIGHT},       (int8_t) c, age},
            {{(float) (i + 1) / (float) B_WIDTH, (float) j / (float) B_HEIGHT},       (int8_t) c, age},
            {{(float) i / (float) B_WIDTH,       (float) (j + 1) / (float) B_HEIGHT}, (int8_t) c, age},
            {{(float) (i + 1) / (float) B_WIDTH, (float) (j + 1) / (float) B_HEIGHT}, (int8_t) c, age},
            {{(float) i / (float) B_WIDTH,       (float) (j + 1) / (float) B_HEIGHT}, (int8_t) c, age},
            {{(float) (i + 1) / (float) B_WIDTH, (float) j / (float) B_HEIGHT},       (int8_t) c, age}
    };
}

/**
 * Places a new tetrimino on the board at its current location
 */
void Tetris::placeTetrimino() {
    // Cycle through all tetrimino blocks
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            // And add them to the board if they are not empty
            if ((*TETRIMINO_SHAPES[cursorTetrimino])[cursorRotation][x][y]) {
                int bx = cursorX + x;
                int by = cursorY + y;

                // And valid
                if (!(by < 0 | bx < 0 | bx >= B_WIDTH)) {
                    board[bx][by] = cursorColour;
                }
            }
        }
    }

    // Add to score
    score += 10;
}

/**
 * Checks if the tetrimino can be rotated left
 *
 * @return
 */
void Tetris::rotateLeft() {
    if (canGoHere(0, 0, 1)) {
        cursorRotation = (cursorRotation + 1) % 4;
    }
    // Check if the tetrimino can be moved left
    else if (canGoHere(-1, 0, 1)) {
        cursorX--;
        cursorRotation = (cursorRotation + 1) % 4;
    }
    // Check if the tetrimino can be moved right
    else if (canGoHere(1, 0, 1)) {
        cursorX++;
        cursorRotation = (cursorRotation + 1) % 4;
    }
    // Check if the tetrimino can be moved left
    else if (canGoHere(-2, 0, 1)) {
        cursorX--;
        cursorX--;
        cursorRotation = (cursorRotation + 1) % 4;
    }
    // Check if the tetrimino can be moved right
    else if (canGoHere(2, 0, 1)) {
        cursorX++;
        cursorX++;
        cursorRotation = (cursorRotation + 1) % 4;
    }
}

/**
 * Checks if the tetrimino can be moved down
 *
 * @return
 */
bool Tetris::canGoDown() {
    return canGoHere(0, -1);
}

/**
 * Checks if the tetrimino can be moved left
 *
 * @return
 */
bool Tetris::canGoLeft() {
    return canGoHere(-1, 0);
}

/**
 * Checks if the tetrimino can be moved right
 *
 * @return
 */
bool Tetris::canGoRight() {
    return canGoHere(1, 0);
}

/**
 * Checks if the tetrimino can be placed at the given offset and rotation
 *
 * @param ox
 * @param oy
 * @param r
 * @return
 */
bool Tetris::canGoHere(int8_t ox, int8_t oy, int r) {
    int cr = (cursorRotation + r) % 4;

    /* ITERATE OVER TETRIMINO BLOCKS */
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if ((*TETRIMINO_SHAPES[cursorTetrimino])[cr][x][y]) {
                int bx = cursorX + x + ox;
                int by = cursorY + y + oy;

                // Check if cell is out of bounds or collides with another block
                if (by < 0 | bx < 0 | bx >= B_WIDTH) {
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

/**
 * Resets the cursor to the top middle and sets a new random tetrimino
 */
void Tetris::newTetrimino() {
    // Set cursor to top middle
    cursorX = B_WIDTH / 2;
    cursorY = B_HEIGHT - 1;

    // Set cursor to random tetrimino
    cursorTetrimino = randomTetrimino();
    cursorColour = randomColour();
    cursorRotation = 0;
}

/**
 * Clears full row on the board
 */
void Tetris::clearRows() {
    uint8_t nClears = 0;
    for (int i = 0; i < B_HEIGHT; i++) {
        bool rowFull = true;
        // Check if row is full
        for (auto &row: board) {
            if (!row[i]) {
                rowFull = false;
                break;
            }
        }
        if (rowFull) {
            // Clear row
            for (auto &row: board) {
                row[i] = Colour::NONE;
            }
            // Move all rows above down
            for (int j = i; j < B_HEIGHT - 1; j++) {
                for (auto &k: board) {
                    k[j] = k[j + 1];
                }
            }
            i--;
            nClears++;
        }
    }

    // Add score based on number of clears
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

/**
 * Clears the board and resets the score
 */
void Tetris::clearBoard() {
    // Reset score
    score = 0;
    ticks = 0;

    // Reset timers
    nextLeftTick = 0;
    nextRightTick = 0;
    nextDownTick = 0;
    nextUpTick = 0;
    nextSpaceTick = 0;
    nextSpeedUpdateTick = 0;

    // Reset Speed
    iDropTickDivider = 3;

    // Clear board
    for (auto &row: board) {
        for (auto &cell: row) {
            cell = Colour::NONE;
        }
    }
}

/**
 * Returns a random colour out of a bag of colours
 *
 * @return
 */
Tetris::Colour Tetris::randomColour() {
    // Init bag of colours
    static std::vector<Colour> bag = {
            Colour::RED, Colour::GREEN, Colour::BLUE, Colour::YELLOW, Colour::ORANGE, Colour::PURPLE
    };

    // Choose random colour
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

/**
 * Returns a random tetrimino out of a bag of tetriminos
 *
 * @return
 */
Tetris::Tetriminos Tetris::randomTetrimino() {
    // Init bag of tetriminos
    static std::vector<Tetriminos> bag = {
            Tetriminos::I, Tetriminos::J, Tetriminos::L, Tetriminos::O, Tetriminos::S, Tetriminos::T, Tetriminos::Z
    };

    // Choose random tetrimino
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
