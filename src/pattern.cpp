#include "pattern.hpp"
#include <algorithm>

// Get score for each pattern type
int PatternDetector::getPatternScore(PatternType type) {
    switch (type) {
        case PatternType::FIVE:        return 100000;  // Win
        case PatternType::OPEN_FOUR:   return 10000;   // Must block
        case PatternType::FOUR:        return 5000;    // Strong threat
        case PatternType::OPEN_THREE:  return 1000;    // Good threat
        case PatternType::THREE:       return 500;     // Weak threat
        case PatternType::OPEN_TWO:    return 100;     // Potential
        case PatternType::TWO:         return 50;      // Weak potential
        case PatternType::ONE:         return 10;      // Very weak
        default:                       return 0;
    }
}

// Analyze a single direction for patterns
int PatternDetector::analyzeDirection(const Board& board, int x, int y, int dx, int dy, Cell player) {
    int count = 1;  // Count the piece at (x, y)
    bool leftOpen = false;
    bool rightOpen = false;
    
    // Count in positive direction
    int nx = x + dx;
    int ny = y + dy;
    int spaces = 0;
    
    while (nx >= 0 && nx < 20 && ny >= 0 && ny < 20 && spaces < 1) {
        Cell cell = board.getCell(nx, ny);
        if (cell == player) {
            count++;
            nx += dx;
            ny += dy;
        } else if (cell == Cell::EMPTY && spaces == 0) {
            spaces++;
            rightOpen = true;
            nx += dx;
            ny += dy;
        } else {
            break;
        }
    }
    
    // Count in negative direction
    nx = x - dx;
    ny = y - dy;
    spaces = 0;
    
    while (nx >= 0 && nx < 20 && ny >= 0 && ny < 20 && spaces < 1) {
        Cell cell = board.getCell(nx, ny);
        if (cell == player) {
            count++;
            nx -= dx;
            ny -= dy;
        } else if (cell == Cell::EMPTY && spaces == 0) {
            spaces++;
            leftOpen = true;
            nx -= dx;
            ny -= dy;
        } else {
            break;
        }
    }
    
    // Score based on count and openness
    int score = 0;
    
    if (count >= 5) {
        score = getPatternScore(PatternType::FIVE);
    } else if (count == 4) {
        if (leftOpen && rightOpen) {
            score = getPatternScore(PatternType::OPEN_FOUR);
        } else if (leftOpen || rightOpen) {
            score = getPatternScore(PatternType::FOUR);
        }
    } else if (count == 3) {
        if (leftOpen && rightOpen) {
            score = getPatternScore(PatternType::OPEN_THREE);
        } else if (leftOpen || rightOpen) {
            score = getPatternScore(PatternType::THREE);
        }
    } else if (count == 2) {
        if (leftOpen && rightOpen) {
            score = getPatternScore(PatternType::OPEN_TWO);
        } else if (leftOpen || rightOpen) {
            score = getPatternScore(PatternType::TWO);
        }
    } else if (count == 1) {
        if (leftOpen || rightOpen) {
            score = getPatternScore(PatternType::ONE);
        }
    }
    
    return score;
}

// Analyze all patterns for a position
Pattern PatternDetector::analyzePosition(const Board& board, int x, int y, Cell player) {
    Pattern result;
    result.count = 0;
    result.score = 0;
    result.type = PatternType::ONE;
    
    // Check all 4 directions
    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    
    int maxScore = 0;
    for (int i = 0; i < 4; i++) {
        int score = analyzeDirection(board, x, y, directions[i][0], directions[i][1], player);
        if (score > maxScore) {
            maxScore = score;
        }
        result.score += score;
    }
    
    return result;
}

// Check if position has open four
bool PatternDetector::hasOpenFour(const Board& board, int x, int y, Cell player) {
    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    
    for (int i = 0; i < 4; i++) {
        int count = board.countConsecutive(x, y, directions[i][0], directions[i][1], player);
        if (count == 4) {
            // Check if both ends are open
            int x1 = x - directions[i][0];
            int y1 = y - directions[i][1];
            int x2 = x + directions[i][0] * 4;
            int y2 = y + directions[i][1] * 4;
            
            bool open1 = (x1 >= 0 && x1 < 20 && y1 >= 0 && y1 < 20 && 
                         board.getCell(x1, y1) == Cell::EMPTY);
            bool open2 = (x2 >= 0 && x2 < 20 && y2 >= 0 && y2 < 20 && 
                         board.getCell(x2, y2) == Cell::EMPTY);
            
            if (open1 && open2) {
                return true;
            }
        }
    }
    return false;
}

// Check if position has open three
bool PatternDetector::hasOpenThree(const Board& board, int x, int y, Cell player) {
    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    
    for (int i = 0; i < 4; i++) {
        int count = board.countConsecutive(x, y, directions[i][0], directions[i][1], player);
        if (count == 3) {
            int x1 = x - directions[i][0];
            int y1 = y - directions[i][1];
            int x2 = x + directions[i][0] * 3;
            int y2 = y + directions[i][1] * 3;
            
            bool open1 = (x1 >= 0 && x1 < 20 && y1 >= 0 && y1 < 20 && 
                         board.getCell(x1, y1) == Cell::EMPTY);
            bool open2 = (x2 >= 0 && x2 < 20 && y2 >= 0 && y2 < 20 && 
                         board.getCell(x2, y2) == Cell::EMPTY);
            
            if (open1 && open2) {
                return true;
            }
        }
    }
    return false;
}

// Check if position has four (not necessarily open)
bool PatternDetector::hasFour(const Board& board, int x, int y, Cell player) {
    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    
    for (int i = 0; i < 4; i++) {
        int count = board.countConsecutive(x, y, directions[i][0], directions[i][1], player);
        if (count >= 4) {
            return true;
        }
    }
    return false;
}

// Check if position has three
bool PatternDetector::hasThree(const Board& board, int x, int y, Cell player) {
    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    
    for (int i = 0; i < 4; i++) {
        int count = board.countConsecutive(x, y, directions[i][0], directions[i][1], player);
        if (count >= 3) {
            return true;
        }
    }
    return false;
}

// Evaluate all patterns on the board for a player
int PatternDetector::evaluatePatterns(const Board& board, Cell player) {
    int totalScore = 0;
    
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            if (board.getCell(x, y) == player) {
                Pattern pattern = analyzePosition(board, x, y, player);
                totalScore += pattern.score;
            }
        }
    }
    
    return totalScore;
}

// Check if a move creates a double threat
bool PatternDetector::isDoubleThreat(const Board& board, int x, int y, Cell player) {
    Board tempBoard = board;
    tempBoard.placeStone(x, y, player);
    
    int threatCount = 0;
    
    // Check all directions for open threes or fours
    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    
    for (int i = 0; i < 4; i++) {
        int count = tempBoard.countConsecutive(x, y, directions[i][0], directions[i][1], player);
        if (count >= 3) {
            threatCount++;
        }
    }
    
    return threatCount >= 2;
}

// Find all threatening moves
std::vector<Move> PatternDetector::findThreats(const Board& board, Cell player) {
    std::vector<Move> threats;
    
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            if (board.isValidMove(x, y)) {
                Board tempBoard = board;
                tempBoard.placeStone(x, y, player);
                
                if (tempBoard.checkWin(x, y, player) || 
                    hasOpenFour(tempBoard, x, y, player) ||
                    hasOpenThree(tempBoard, x, y, player)) {
                    threats.emplace_back(x, y);
                }
            }
        }
    }
    
    return threats;
}

// Check if a position is open (has empty neighbors)
bool PatternDetector::isOpen(const Board& board, int x, int y) {
    const int offsets[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    
    for (int i = 0; i < 8; i++) {
        int nx = x + offsets[i][0];
        int ny = y + offsets[i][1];
        
        if (nx >= 0 && nx < 20 && ny >= 0 && ny < 20) {
            if (board.getCell(nx, ny) != Cell::EMPTY) {
                return true;
            }
        }
    }
    
    return false;
}
