#include "ai.hpp"
#include <algorithm>
#include <limits>

AI::AI() : nodesEvaluated(0) {
    resetSearchStats();
}

// Get opponent's color
Cell AI::getOpponentColor(Cell player) const {
    return (player == Cell::BLACK) ? Cell::WHITE : Cell::BLACK;
}

// Main entry point - finds best move using Minimax with Alpha-Beta
Move AI::findBestMove(const Board& board, Cell myColor) {
    resetSearchStats();
    startTime = std::chrono::steady_clock::now();
    
    // Check for immediate win
    Move winMove = findImmediateWin(board, myColor);
    if (winMove.first != -1) {
        return winMove;
    }
    
    // Check for immediate threat to block
    Cell opponent = getOpponentColor(myColor);
    Move threatMove = findImmediateWin(board, opponent);
    if (threatMove.first != -1) {
        return threatMove;
    }
    
    // Use iterative deepening with alpha-beta
    return iterativeDeepening(board, myColor);
}

// Find immediate winning move
Move AI::findImmediateWin(const Board& board, Cell myColor) {
    auto moves = getRelevantMoves(board);
    
    for (const auto& move : moves) {
        if (isWinningMove(board, move, myColor)) {
            return move;
        }
    }
    
    return Move(-1, -1);
}

// Find immediate threat
Move AI::findImmediateThreat(const Board& board, Cell myColor) {
    Cell opponent = getOpponentColor(myColor);
    return findImmediateWin(board, opponent);
}

// Iterative deepening with time management
Move AI::iterativeDeepening(const Board& board, Cell myColor) {
    Move bestMove(-1, -1);
    
    // Get candidate moves once
    auto moves = getOrderedMovesAdvanced(board, myColor);
    
    if (moves.empty()) {
        return Move(10, 10); // Center fallback
    }
    
    if (moves.size() == 1) {
        return moves[0];
    }
    
    // Try increasing depths until time runs out
    for (int depth = 2; depth <= MAX_DEPTH; depth++) {
        if (isTimeUp()) break;
        
        int alpha = std::numeric_limits<int>::min();
        int beta = std::numeric_limits<int>::max();
        Move currentBest = bestMove;
        int currentBestScore = std::numeric_limits<int>::min();
        
        for (const auto& move : moves) {
            if (isTimeUp()) break;
            
            Board tempBoard = board;
            tempBoard.placeStone(move.first, move.second, myColor);
            
            int score = -alphaBeta(tempBoard, depth - 1, -beta, -alpha, 
                                   myColor, getOpponentColor(myColor));
            
            if (score > currentBestScore) {
                currentBestScore = score;
                currentBest = move;
            }
            
            alpha = std::max(alpha, score);
        }
        
        // Only update if we completed this depth
        if (!isTimeUp()) {
            bestMove = currentBest;
        }
    }
    
    return (bestMove.first != -1) ? bestMove : moves[0];
}

// Alpha-Beta pruning implementation (Negamax variant)
int AI::alphaBeta(Board& board, int depth, int alpha, int beta, 
                  Cell maximizingPlayer, Cell currentPlayer) {
    nodesEvaluated++;
    
    // Time check
    if (isTimeUp()) {
        return evaluatePositionAdvanced(board, maximizingPlayer);
    }
    
    // Terminal conditions
    if (depth == 0) {
        return evaluatePositionAdvanced(board, maximizingPlayer) * 
               (currentPlayer == maximizingPlayer ? 1 : -1);
    }
    
    // Check for win
    for (int y = 0; y < 20 && !isTimeUp(); y++) {
        for (int x = 0; x < 20; x++) {
            if (board.getCell(x, y) == currentPlayer) {
                if (board.checkWin(x, y, currentPlayer)) {
                    return (currentPlayer == maximizingPlayer ? 100000 : -100000);
                }
            }
        }
    }
    
    // Get ordered moves
    auto moves = getOrderedMovesAdvanced(board, currentPlayer);
    
    if (moves.empty()) {
        return evaluatePositionAdvanced(board, maximizingPlayer) * 
               (currentPlayer == maximizingPlayer ? 1 : -1);
    }
    
    int maxScore = std::numeric_limits<int>::min();
    Cell nextPlayer = getOpponentColor(currentPlayer);
    
    for (const auto& move : moves) {
        if (isTimeUp()) break;
        
        board.placeStone(move.first, move.second, currentPlayer);
        
        int score = -alphaBeta(board, depth - 1, -beta, -alpha, 
                               maximizingPlayer, nextPlayer);
        
        // Undo move (need to implement in Board class)
        board.placeStone(move.first, move.second, Cell::EMPTY);
        
        maxScore = std::max(maxScore, score);
        alpha = std::max(alpha, score);
        
        if (alpha >= beta) {
            break; // Beta cutoff
        }
    }
    
    return maxScore;
}

// Advanced position evaluation
int AI::evaluatePositionAdvanced(const Board& board, Cell maximizingPlayer) {
    Cell opponent = getOpponentColor(maximizingPlayer);
    
    // Evaluate patterns for both players
    int myScore = PatternDetector::evaluatePatterns(board, maximizingPlayer);
    int opponentScore = PatternDetector::evaluatePatterns(board, opponent);
    
    // Weight defense slightly higher
    return myScore - (opponentScore * 1.1);
}

// Basic evaluation (fallback)
int AI::evaluatePosition(const Board& board, Cell maximizingPlayer) {
    return evaluatePositionAdvanced(board, maximizingPlayer);
}

// Get relevant moves (reduce search space)
std::vector<Move> AI::getRelevantMoves(const Board& board) {
    std::vector<Move> relevantMoves;
    
    // Only consider moves near existing stones
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            if (board.getCell(x, y) == Cell::EMPTY) {
                // Check if there's a stone within 2 squares
                bool hasNeighbor = false;
                
                for (int dy = -2; dy <= 2 && !hasNeighbor; dy++) {
                    for (int dx = -2; dx <= 2; dx++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        
                        if (nx >= 0 && nx < 20 && ny >= 0 && ny < 20) {
                            if (board.getCell(nx, ny) != Cell::EMPTY) {
                                hasNeighbor = true;
                                break;
                            }
                        }
                    }
                }
                
                if (hasNeighbor || board.getMoveCount() == 0) {
                    relevantMoves.emplace_back(x, y);
                }
            }
        }
    }
    
    // If board is empty, start in center
    if (relevantMoves.empty()) {
        relevantMoves.emplace_back(10, 10);
    }
    
    return relevantMoves;
}

// Advanced move ordering for better alpha-beta pruning
std::vector<Move> AI::getOrderedMovesAdvanced(const Board& board, Cell myColor) {
    auto moves = getRelevantMoves(board);
    Cell opponent = getOpponentColor(myColor);
    
    // Score each move
    std::vector<std::pair<Move, int>> scoredMoves;
    
    for (const auto& move : moves) {
        int score = 0;
        
        // Check if winning move
        if (isWinningMove(board, move, myColor)) {
            score += 1000000;
        }
        
        // Check if blocks opponent win
        if (isWinningMove(board, move, opponent)) {
            score += 500000;
        }
        
        // Check pattern strength
        Board tempBoard = board;
        tempBoard.placeStone(move.first, move.second, myColor);
        
        if (PatternDetector::hasOpenFour(tempBoard, move.first, move.second, myColor)) {
            score += 100000;
        }
        if (PatternDetector::hasOpenThree(tempBoard, move.first, move.second, myColor)) {
            score += 10000;
        }
        if (PatternDetector::hasFour(tempBoard, move.first, move.second, myColor)) {
            score += 50000;
        }
        if (PatternDetector::hasThree(tempBoard, move.first, move.second, myColor)) {
            score += 5000;
        }
        
        // Check opponent patterns (defense)
        tempBoard = board;
        tempBoard.placeStone(move.first, move.second, opponent);
        
        if (PatternDetector::hasOpenFour(tempBoard, move.first, move.second, opponent)) {
            score += 80000;
        }
        if (PatternDetector::hasOpenThree(tempBoard, move.first, move.second, opponent)) {
            score += 8000;
        }
        
        // Prefer center
        int centerDist = std::abs(move.first - 10) + std::abs(move.second - 10);
        score += (40 - centerDist) * 10;
        
        scoredMoves.emplace_back(move, score);
    }
    
    // Sort by score (descending)
    std::sort(scoredMoves.begin(), scoredMoves.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Extract moves
    std::vector<Move> orderedMoves;
    for (const auto& sm : scoredMoves) {
        orderedMoves.push_back(sm.first);
    }
    
    // Limit to top moves for deep searches
    if (orderedMoves.size() > 20) {
        orderedMoves.resize(20);
    }
    
    return orderedMoves;
}

// Simple move ordering (fallback)
std::vector<Move> AI::getOrderedMoves(const Board& board, Cell myColor) {
    return getOrderedMovesAdvanced(board, myColor);
}

int AI::getMoveScore(const Board& board, Move move, Cell myColor) {
    (void)board;
    (void)myColor;
    
    // Distance from center
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
    return isWinningMove(board, move, opponentColor);
}

void AI::resetSearchStats() {
    nodesEvaluated = 0;
    transpositionTable.clear();
}

bool AI::isTimeUp() const {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - startTime);
    return elapsed.count() >= MAX_TIME_MS;
}

uint64_t AI::hashBoard(const Board& board) const {
    uint64_t hash = 0;
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            hash ^= static_cast<uint64_t>(board.getCell(x, y)) << ((y * 20 + x) % 64);
        }
    }
    return hash;
}