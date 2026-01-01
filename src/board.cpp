#include "board.hpp"
#include <iostream>

Board::Board() : moveCount(0) {
    clear();
}

void Board::clear() {
    for (auto& row : grid) {
        row.fill(Cell::EMPTY);
    }
    moveCount = 0;
}

bool Board::isValidMove(int x, int y) const {
    return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE &&
            getCell(x, y) == Cell::EMPTY);
}

Cell Board::getCell(int x, int y) const {
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
        return grid[y][x];
    }
    return Cell::EMPTY;
}

bool Board::placeStone(int x, int y, Cell stone) {
    if (stone == Cell::EMPTY) {
        // Allow placing empty (for undo)
        if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
            if (grid[y][x] != Cell::EMPTY) {
                moveCount--;
            }
            grid[y][x] = Cell::EMPTY;
            return true;
        }
        return false;
    }
    
    if (!isValidMove(x, y)) {
        return false;
    }
    grid[y][x] = stone;
    moveCount++;
    return true;
}

bool Board::removeStone(int x, int y) {
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
        if (grid[y][x] != Cell::EMPTY) {
            grid[y][x] = Cell::EMPTY;
            moveCount--;
            return true;
        }
    }
    return false;
}

bool Board::checkWin(int x, int y, Cell stone) const {
    if (stone == Cell::EMPTY) {
        return false;
    }

    // Check all 4 directions
    const int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}
    };

    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];

        int count = countConsecutive(x, y, dx, dy, stone);

        if (count >= 5) {
            return true;
        }
    }

    return false;
}

int Board::countConsecutive(int x, int y, int dx, int dy, Cell stone) const {
    int count = 1;

    // Count in positive direction
    int nx = x + dx;
    int ny = y + dy;
    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE &&
           getCell(nx, ny) == stone) {
        count++;
        nx += dx;
        ny += dy;
    }

    // Count in negative direction
    nx = x - dx;
    ny = y - dy;
    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE &&
           getCell(nx, ny) == stone) {
        count++;
        nx -= dx;
        ny -= dy;
    }

    return count;
}

std::vector<Move> Board::getAvailableMoves() const {
    std::vector<Move> moves;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (getCell(x, y) == Cell::EMPTY) {
                moves.emplace_back(x, y);
            }
        }
    }
    return moves;
}

bool Board::isBoardFull() const {
    return moveCount >= BOARD_SIZE * BOARD_SIZE;
}

void Board::printBoard() const {
    std::cout << std::endl;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            std::cout << static_cast<char>(getCell(x, y)) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}