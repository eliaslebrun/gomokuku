#include "ai.hpp"
#include <algorithm>
#include <limits>

AI::AI() : nodesEvaluated(0) {
    resetSearchStats();
}

Move AI::findBestMove(const Board& board, Cell myColor) {
    (void)myColor; // Suppress unused warning for now
    
    resetSearchStats();
    startTime = std::chrono::steady_clock::now();
    
    // Placeholder: return first available move
    auto availableMoves = board.getAvailableMoves();
    if (!availableMoves.empty()) {
        return availableMoves[0];
    }
    
    // Should not reach here, but fallback
    return Move(9, 9);
}

int AI::evaluatePosition(const Board& board, Cell maximizingPlayer) {
    (void)board; // Suppress unused warning
    (void)maximizingPlayer; // Suppress unused warning
    
    ++nodesEvaluated;
    
    // Placeholder: random evaluation
    return 0;
}

int AI::alphaBeta(Board board, int depth, int alpha, int beta, Cell maximizingPlayer) {
    if (depth == 0 || board.checkWin(0, 0, maximizingPlayer) || isTimeUp()) {
        return evaluatePosition(board, maximizingPlayer);
    }
    
    (void)alpha; // Suppress unused warning
    (void)beta; // Suppress unused warning
    
    // Placeholder implementation
    return 0;
}

int AI::minimax(Board board, int depth, bool isMaximizing, Cell maximizingPlayer) {
    if (depth == 0 || board.checkWin(0, 0, maximizingPlayer) || isTimeUp()) {
        return evaluatePosition(board, maximizingPlayer);
    }
    
    (void)isMaximizing; // Suppress unused warning
    
    // Placeholder
    return 0;
}

std::vector<Move> AI::getOrderedMoves(const Board& board) {
    auto moves = board.getAvailableMoves();
    
    // Placeholder: basic center-first ordering
    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
        int centerA = std::abs(a.first - 10) + std::abs(a.second - 10);
        int centerB = std::abs(b.first - 10) + std::abs(b.second - 10);
        return centerA < centerB;
    });
    
    return moves;
}

int AI::getMoveScore(const Board& board, Move move, Cell myColor) {
    (void)board; // Suppress unused warning
    (void)myColor; // Suppress unused warning
    
    // Placeholder: distance from center
    int centerX = 10;
    int centerY = 10;
    return -(std::abs(move.first - centerX) + std::abs(move.second - centerY));
}

bool AI::isWinningMove(const Board& board, Move move, Cell myColor) {
    Board tempBoard = board;
    tempBoard.placeStone(move.first, move.second, myColor);
    return tempBoard.checkWin(move.first, move.second, myColor);
}

bool AI::isThreatBlocking(const Board& board, Move move, Cell opponentColor) {
    Board tempBoard = board;
    tempBoard.placeStone(move.first, move.second, opponentColor);
    return tempBoard.checkWin(move.first, move.second, opponentColor);
}

void AI::resetSearchStats() {
    nodesEvaluated = 0;
}

bool AI::isTimeUp() const {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - startTime);
    return elapsed.count() >= MAX_TIME_MS;
}