#ifndef PATTERN_HPP
#define PATTERN_HPP

#include "board.hpp"
#include <vector>

// Pattern types and their scores
enum class PatternType {
    FIVE,           // 5 in a row (win)
    OPEN_FOUR,      // _XXXX_ (must block)
    FOUR,           // XXXX_ or _XXXX
    OPEN_THREE,     // _XXX_ (threat)
    THREE,          // XXX_ or _XXX
    OPEN_TWO,       // _XX_
    TWO,            // XX_ or _XX
    ONE             // X
};

struct Pattern {
    PatternType type;
    int count;
    int score;
};

class PatternDetector {
    public:
        // Pattern detection
        static Pattern analyzePosition(const Board& board, int x, int y, Cell player);
        static int countPattern(const Board& board, int x, int y, int dx, int dy, Cell player, int length);

        // Advanced pattern detection
        static bool hasOpenFour(const Board& board, int x, int y, Cell player);
        static bool hasOpenThree(const Board& board, int x, int y, Cell player);
        static bool hasFour(const Board& board, int x, int y, Cell player);
        static bool hasThree(const Board& board, int x, int y, Cell player);

        // Pattern scoring
        static int getPatternScore(PatternType type);
        static int evaluatePatterns(const Board& board, Cell player);

        // Threat detection
        static bool isDoubleThreat(const Board& board, int x, int y, Cell player);
        static std::vector<Move> findThreats(const Board& board, Cell player);

        // Helper functions
        static int analyzeDirection(const Board& board, int x, int y, int dx, int dy, Cell player);
        static bool isOpen(const Board& board, int x, int y);
};

#endif // PATTERN_HPP
