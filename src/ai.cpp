/*
** AI Engine Implementation
** Goal: Minimize search space while maximizing winning probability
*/

#include "ai.hpp"
#include <algorithm>
#include <limits>

AI::AI() : nodesEvaluated(0) {
    resetSearchStats();
}

Move AI::findBestMove(const Board& board, Cell myColor) {
    // TODO: Implement move selection algorithm
    // - Use alpha-beta pruning or minimax
    // - Consider time constraints (5 seconds max)
    // - Return best move found
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
    // TODO: Implement position evaluation
    // - Count patterns and threats
    // - Consider board control
    // - Return score (positive for maximizing player)
    ++nodesEvaluated;

    // Placeholder: random evaluation
    return 0;
}

int AI::alphaBeta(Board board, int depth, int alpha, int beta, Cell maximizingPlayer) {
    // TODO: Implement alpha-beta pruning search algorithm
    // - Depth-limited search
    // - Alpha-beta cutoffs
    // - Return best evaluation
    if (depth == 0 || board.checkWin(0, 0, maximizingPlayer) || isTimeUp()) {
        return evaluatePosition(board, maximizingPlayer);
    }

    // Placeholder implementation
    return 0;
}

int AI::minimax(Board board, int depth, bool isMaximizing, Cell maximizingPlayer) {
    // TODO: Implement minimax algorithm (if preferred over alpha-beta)
    // - Standard minimax with depth limit
    // - Consider all possible moves
    // - Return best evaluation

    if (depth == 0 || board.checkWin(0, 0, maximizingPlayer) || isTimeUp()) {
        return evaluatePosition(board, maximizingPlayer);
    }

    // Placeholder
    return 0;
}

std::vector<Move> AI::getOrderedMoves(const Board& board) {
    // TODO: Implement move ordering for efficient search
    // - Prioritize center moves
    // - Consider known good positions
    // - Return ordered list
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
    // TODO: Heuristic move scoring for ordering
    // - Rating based on threat potential
    // - Center proximity bonus
    // - Return numeric score

    // Placeholder: distance from center
    int centerX = 10, centerY = 10;
    return -(std::abs(move.first - centerX) + std::abs(move.second - centerY));
}

bool AI::isWinningMove(const Board& board, Move move, Cell myColor) {
    // TODO: Check if moving here creates 5 in a row
    Board tempBoard = board;
    tempBoard.placeStone(move.first, move.second, myColor);
    return tempBoard.checkWin(move.first, move.second, myColor);
}

bool AI::isThreatBlocking(const Board& board, Move move, Cell opponentColor) {
    // TODO: Check if this move blocks opponent's threat
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
