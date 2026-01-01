#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <vector>
#include <utility>

enum class Cell : char {
    EMPTY = '.',
    BLACK = 'X',
    WHITE = 'O'
};

using Position = std::pair<int, int>;
using Move = std::pair<int, int>;

class Board {
    private:
        // Board state constants
        static const int BOARD_SIZE = 20;

        // Board representation
        std::array<std::array<Cell, BOARD_SIZE>, BOARD_SIZE> grid;

        // Game state
        int moveCount;

    public:
        // Constructor
        Board();

        // Board operations
        bool placeStone(int x, int y, Cell stone);
        bool removeStone(int x, int y); // For undoing moves in minimax
        bool isValidMove(int x, int y) const;
        Cell getCell(int x, int y) const;
        void clear();

        // Game logic
        bool checkWin(int x, int y, Cell stone) const;
        std::vector<Move> getAvailableMoves() const;
        bool isBoardFull() const;

        // Utility functions
        void printBoard() const;
        int getBoardSize() const { return BOARD_SIZE; }
        int getMoveCount() const { return moveCount; }

        // Helper for detection algorithms
        int countConsecutive(int x, int y, int dx, int dy, Cell stone) const;
};

#endif // BOARD_HPP
