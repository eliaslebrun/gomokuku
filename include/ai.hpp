#ifndef AI_HPP
#define AI_HPP

#include <chrono>
#include <vector>
#include <unordered_map>
#include "board.hpp"
#include "pattern.hpp"

class AI {
    private:
        // AI configuration
        static const int MAX_DEPTH = 6; // Search depth limit
        static const int MAX_TIME_MS = 4900; // Stay under 5 seconds

        // Search state
        int nodesEvaluated;
        std::chrono::steady_clock::time_point startTime;

        // Transposition table for memoization (optional future optimization)
        std::unordered_map<uint64_t, int> transpositionTable;

    public:
        // Constructor
        AI();

        // Main AI interface
        Move findBestMove(const Board& board, Cell myColor);

        // Evaluation function
        int evaluatePosition(const Board& board, Cell maximizingPlayer);
        int evaluatePositionAdvanced(const Board& board, Cell maximizingPlayer);

        // Minimax + Alpha-Beta
        int alphaBeta(Board& board, int depth, int alpha, int beta,
                      Cell maximizingPlayer, Cell currentPlayer);

        // Iterative deepening
        Move iterativeDeepening(const Board& board, Cell myColor);

        // Move ordering and heuristics
        std::vector<Move> getOrderedMoves(const Board& board, Cell myColor);
        std::vector<Move> getOrderedMovesAdvanced(const Board& board, Cell myColor);
        int getMoveScore(const Board& board, Move move, Cell myColor);

        // Smart move reduction (only consider relevant moves)
        std::vector<Move> getRelevantMoves(const Board& board);

        // Pattern recognition
        bool isWinningMove(const Board& board, Move move, Cell myColor);
        bool isThreatBlocking(const Board& board, Move move, Cell opponentColor);
        Move findImmediateWin(const Board& board, Cell myColor);
        Move findImmediateThreat(const Board& board, Cell myColor);

        // Utility functions
        void resetSearchStats();
        int getNodesEvaluated() const { return nodesEvaluated; }
        bool isTimeUp() const;
        uint64_t hashBoard(const Board& board) const;

        // Helper
        Cell getOpponentColor(Cell player) const;
};

#endif // AI_HPP
