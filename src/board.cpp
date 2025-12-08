/*
** Board Implementation
** Gomoku board state management and game logic
*/

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
        return grid[y][x]; // Note: array[y][x] for row-major order
    }
    return Cell::EMPTY;
}

bool Board::placeStone(int x, int y, Cell stone) {
    if (!isValidMove(x, y)) {
        return false;
    }
    grid[y][x] = stone;
    moveCount++;
    return true;
}

bool Board::checkWin(int x, int y, Cell stone) const {
    // TODO: Implement win detection (5 in a row)
    // Check horizontal, vertical, and both diagonals
    // Use countConsecutive helper
    return false; // Placeholder
}

int Board::countConsecutive(int x, int y, int dx, int dy, Cell stone) const {
    // TODO: Count consecutive stones in a direction (dx,dy)
    int count = 1; // Current stone

    // Count in positive direction
    int nx = x + dx, ny = y + dy;
    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE &&
           getCell(nx, ny) == stone) {
        count++;
        nx += dx;
        ny += dy;
    }

    // Count in negative direction
    nx = x - dx, ny = y - dy;
    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE &&
           getCell(nx, ny) == stone) {
        count++;
        nx -= dx;
        ny -= dy;
    }

    return count;
}

std::vector<Move> Board::getAvailableMoves() const {
    // TODO: Return list of all empty positions
    // Optimize by checking nearby occupied cells
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
    // TODO: Debug function to print the board state
    std::cout << std::endl;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            std::cout << static_cast<char>(getCell(x, y)) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
