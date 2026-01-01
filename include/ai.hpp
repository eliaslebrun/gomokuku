#ifndef AI_HPP
#define AI_HPP

#include <chrono>
#include "board.hpp"

class AI {
private:
    // AI configuration
    static const int MAX_DEPTH = 5; // Search depth limit
    static const int MAX_TIME_MS = 4900; // Stay under 5 seconds

    // Search state
    int nodesEvaluated;
    std::chrono::steady_clock::time_point startTime;

public:
    // Constructor
    AI();

    // Main AI interface
    Move findBestMove(const Board& board, Cell myColor);

    // Evaluation function
    int evaluatePosition(const Board& board, Cell maximizingPlayer);

    // Search algorithms
    int alphaBeta(Board board, int depth, int alpha, int beta, Cell maximizingPlayer);
    int minimax(Board board, int depth, bool isMaximizing, Cell maximizingPlayer);

    // Move ordering and heuristics
    std::vector<Move> getOrderedMoves(const Board& board);
    int getMoveScore(const Board& board, Move move, Cell myColor);

    // Pattern recognition
    bool isWinningMove(const Board& board, Move move, Cell myColor);
    bool isThreatBlocking(const Board& board, Move move, Cell opponentColor);

    // Utility functions
    void resetSearchStats();
    int getNodesEvaluated() const { return nodesEvaluated; }
    bool isTimeUp() const;
};

#endif // AI_HPP
