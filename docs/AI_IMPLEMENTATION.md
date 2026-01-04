# Gomoku AI Documentation

## Overview

This AI implements **Minimax with Alpha-Beta Pruning** algorithm to play Gomoku.

## Architecture

### Created/Modified Files

#### New files
- `include/pattern.hpp` - Pattern detection (alignments)
- `src/pattern.cpp` - Pattern detection implementation

#### Modified files
- `include/ai.hpp` - Complete AI interface
- `src/ai.cpp` - Algorithm implementation
- `include/board.hpp` - Added `removeStone()`
- `src/board.cpp` - Stone removal implementation

---

## Algorithm Implemented

### Minimax with Alpha-Beta Pruning

**Principle**: Explores the game tree by anticipating opponent moves and choosing the best move.

**Optimizations**:
- ✅ **Alpha-Beta Pruning**: Cuts unnecessary branches (~10x speedup)
- ✅ **Iterative Deepening**: Progressively increases depth (2 → 3 → 4 → 5 → 6)
- ✅ **Move Ordering**: Prioritizes promising moves to maximize cutoffs
- ✅ **Time Management**: Stops before 5 seconds (limit at 4.9s)
- ✅ **Search Space Reduction**: Only considers moves near existing stones

**Depth**: 
- Max depth: 6 (configurable via `MAX_DEPTH`)
- Reached dynamically according to available time

**Advantages**:
- Very good for tactical positions (direct threats)
- Always finds optimal move within search tree
- Predictable performance

**Disadvantages**:
- Combinatorial explosion on 20×20 board
- Horizon effect (doesn't see beyond max depth)

---

## Pattern Detection System

### Recognized Patterns

| Pattern | Description | Score | Example |
|---------|-------------|-------|---------|
| `FIVE` | 5 in a row (win) | 100,000 | `XXXXX` |
| `OPEN_FOUR` | Open 4 in a row | 10,000 | `_XXXX_` |
| `FOUR` | 4 in a row | 5,000 | `XXXX_` |
| `OPEN_THREE` | Open 3 in a row | 1,000 | `_XXX_` |
| `THREE` | 3 in a row | 500 | `XXX_` |
| `OPEN_TWO` | Open 2 in a row | 100 | `_XX_` |
| `TWO` | 2 in a row | 50 | `XX_` |
| `ONE` | 1 stone | 10 | `X` |

### Main Functions

```cpp
// Analyze a position
Pattern pattern = PatternDetector::analyzePosition(board, x, y, player);

// Evaluate entire board
int score = PatternDetector::evaluatePatterns(board, player);

// Threat detection
bool threat = PatternDetector::hasOpenFour(board, x, y, player);
std::vector<Move> threats = PatternDetector::findThreats(board, player);
```

---

## Evaluation Function

**Formula**:
```
Score = MyScore - (OpponentScore × 1.1)
```

Defense is slightly favored (coefficient 1.1) because it's often more important to block the opponent.

**Evaluation based on**:
- Number and type of patterns for each player
- Pattern openness (free squares at ends)
- Stone positions

---

## Move Ordering Optimization

**Priority order**:
1. 🏆 **Winning moves**: Score +1,000,000
2. 🛡️ **Block opponent win**: Score +500,000
3. ⚡ **Create "open 4"**: Score +100,000
4. ⚔️ **Block opponent "open 4"**: Score +80,000
5. 🎯 **Create "4"**: Score +50,000
6. 🔥 **Create "open 3"**: Score +10,000
7. 🛡️ **Block opponent "open 3"**: Score +8,000
8. 📍 **Center proximity**: +10 × (40 - distance)

**Result**: Best moves are explored first, maximizing Alpha-Beta cutoffs.

---

## Search Space Reduction

Instead of considering all **400 squares** on the 20×20 board, the AI only considers:
- Squares at **distance ≤ 2** from an existing stone
- Center (10, 10) if board is empty

**Gain**: Reduces from 400 to ~20-50 candidate moves on average.

---

## Time Management

```cpp
bool AI::isTimeUp() const {
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(
        currentTime - startTime);
    return elapsed.count() >= 4900; // 4.9 seconds
}
```

**Strategy**:
- Limit at 4.9 seconds (safety margin)
- Time checking in all algorithms
- Iterative deepening to maximize reached depth

---

## Tests and Usage

### Basic test
```bash
./pbrain-gomoku-ai
START 20
BEGIN
# AI responds: 10,10 (center)
```

### Test with Piskvork
```bash
# Download piskvork from https://sourceforge.net/projects/piskvork/
# Launch piskvork and add the brain:
# Brain Path: /path/to/pbrain-gomoku-ai
```

---

## Performance

### Time complexity
- **Minimax + Alpha-Beta**: O(b^d) where b = branching factor, d = depth
  - With move ordering: ~O(b^(d/2)) on average

### Space complexity
- **Minimax**: O(d) for call stack

### Memory used
- Estimated: **< 10 MB** (well below 70 MB limit)
- 20×20 board occupies only 400 bytes

---

## Implementation Strengths

✅ **Immediate detection**: Finds winning moves instantly  
✅ **Reactive defense**: Automatically blocks direct threats  
✅ **Robust time management**: Never exceeds 5 seconds  
✅ **Intelligent move ordering**: Explores best moves first  
✅ **Advanced patterns**: Detects all types of alignments  
✅ **Modular code**: Easy to maintain and improve  

---

## Possible Future Improvements

### Short term
- 🔧 Adjust pattern weights (tuning)
- 🔧 Add transposition table (memoization)
- 🔧 Implement opening book (starting positions)

### Long term
- 🚀 Threat-space search
- 🚀 Neural networks for evaluation (deep learning)
- 🚀 Parallel search (multithreading)
- 🚀 Endgame database

---

## References

- **Minimax**: https://en.wikipedia.org/wiki/Minimax
- **Alpha-Beta Pruning**: https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
- **Gomoku Strategy**: https://en.wikipedia.org/wiki/Gomoku

---

## Authors

- **Person 1**: Protocol & Communication
- **Person 2**: Game Logic & Board
- **Person 3**: Artificial Intelligence (you)

**Good luck with the project! 🎮🤖**
