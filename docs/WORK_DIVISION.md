# Gomoku AI Project - Work Division

## Project Overview
Development of a Gomoku AI bot (`pbrain-gomoku-ai`) compliant with the Piskvork protocol, capable of playing on a 20x20 board with freestyle rules.

**Deadline Constraints:**
- 5 seconds maximum per move
- 70 MB memory limit
- Must compile via Makefile on Linux

---

## 👤 Person 1: Communication Protocol & Program Structure

### Responsibilities
Handle all external communication and program infrastructure.

### Tasks
- [ ] **Protocol Implementation (Piskvork)**
  - Implement command parsing (START, TURN, BEGIN, BOARD, etc.)
  - Handle INFO command responses
  - Implement ABOUT command
  - Error handling for invalid commands
  - Standard input/output communication loop

- [ ] **Program Architecture**
  - Main program entry point
  - Command dispatcher/router
  - Integration layer between protocol and game engine
  - Memory management architecture

- [ ] **Build System**
  - Create Makefile with `all`, `re`, `clean`, `fclean` rules
  - Ensure compilation compatibility on Linux
  - Binary naming (`pbrain-gomoku-ai`)

- [ ] **Testing & Integration**
  - Test with piskvork game manager
  - Verify protocol compliance
  - Integration testing with other modules
  - Performance profiling (5-second constraint)

### Estimated Workload: ~33%
**Key Deliverables:** Working I/O system, Makefile, protocol handler

---

## 👤 Person 2: Game Logic & Board Management

### Responsibilities
Core game mechanics, rules enforcement, and data structures.

### Tasks
- [ ] **Board Representation**
  - Design efficient 20x20 board data structure
  - Implement board state management
  - Coordinate system (X, Y positioning)
  - Board state serialization/deserialization

- [ ] **Game Rules Implementation**
  - Move validation (bounds checking, occupied cells)
  - Win condition detection (5 in a row)
  - Check horizontal, vertical, and diagonal sequences
  - Forbidden moves detection (if any)

- [ ] **Move Management**
  - Place stone function
  - Undo/redo move capability (for AI tree search)
  - Available moves generation
  - Quick win/threat pattern detection

- [ ] **Board Analysis Utilities**
  - Pattern recognition helpers
  - Board evaluation functions
  - Threat detection (double-three, double-four, etc.)
  - Helper functions for AI module

### Estimated Workload: ~33%
**Key Deliverables:** Board class/module, rules engine, move validator

---

## 👤 Person 3: AI Strategy & Decision Making

### Responsibilities
Implement the artificial intelligence algorithm that decides the best moves.

### Tasks
- [ ] **Algorithm Selection & Design**
  - Choose AI approach (Minimax, Alpha-Beta, Monte Carlo, Negamax, etc.)
  - Design evaluation function
  - Define search depth strategy
  - Time management (stay under 5 seconds)

- [ ] **AI Core Implementation**
  - Main AI decision function
  - Position evaluation/scoring
  - Pattern recognition (threats, opportunities)
  - Opening book (optional but recommended)

- [ ] **Optimization**
  - Alpha-beta pruning (if using Minimax)
  - Move ordering heuristics
  - Iterative deepening
  - Transposition table (optional)
  - Memory optimization (70 MB limit)

- [ ] **Strategic Intelligence**
  - Attack pattern recognition
  - Defense priority system
  - Winning move detection
  - Threat response logic
  - End-game optimization

### Estimated Workload: ~33%
**Key Deliverables:** AI engine, evaluation function, move selection algorithm

---

## 🤝 Collaboration Points

### Integration Meetings
Schedule regular sync points to ensure modules integrate smoothly:

1. **Week 1:** Architecture review - agree on interfaces between modules
2. **Week 2:** First integration - basic game loop working
3. **Week 3:** Performance tuning and bug fixes
4. **Week 4:** Final testing and optimization

### Shared Responsibilities
- **Documentation:** Each person documents their module
- **Code Review:** Cross-review each other's code
- **Testing:** Create unit tests for your modules
- **Debugging:** Help debug integration issues together

### Module Interfaces (Agreement Required)

**Protocol ↔ Game Logic:**
```
- makeMove(x, y) → bool
- getBoardState() → BoardState
- resetGame()
- isGameOver() → bool
```

**Game Logic ↔ AI:**
```
- getAvailableMoves() → List[Move]
- evaluatePosition() → Score
- isWinningMove(x, y) → bool
- simulateMove(x, y) → void
- undoMove() → void
```

---

## 📊 Workload Balance

| Person | Module | Complexity | Lines of Code (Est.) |
|--------|--------|------------|---------------------|
| Person 1 | Protocol & Structure | Medium | 300-400 |
| Person 2 | Game Logic | Medium-High | 400-500 |
| Person 3 | AI Strategy | High | 400-600 |

**Total Estimated:** 1100-1500 lines of code

---

## 🎯 Milestones

### Milestone 1: Basic Functionality (Week 1-2)
- [ ] Program can start and receive commands
- [ ] Board can be initialized and moves placed
- [ ] Basic AI can make random valid moves
- [ ] **Deliverable:** Bot that can complete a game

### Milestone 2: Smart AI (Week 2-3)
- [ ] AI recognizes winning opportunities
- [ ] AI blocks opponent's winning moves
- [ ] Basic evaluation function working
- [ ] **Deliverable:** Bot that plays intelligently

### Milestone 3: Optimization (Week 3-4)
- [ ] Meets 5-second time constraint consistently
- [ ] Memory usage under 70 MB
- [ ] Wins against basic/medium bots
- [ ] **Deliverable:** Competitive bot

### Milestone 4: Polish (Week 4)
- [ ] Code cleanup and documentation
- [ ] All Makefile rules working
- [ ] Thorough testing completed
- [ ] Optional: Brawl tournament participation
- [ ] **Deliverable:** Production-ready bot

---

## 📝 Optional: Brawl Tournament

If the team decides to participate:
- Create `.brawl` file at repository root
- Choose AI name (5-16 characters, alphanumeric + spaces/dashes/underscores)
- Ensure `make clean` doesn't delete the binary
- Monitor performance on https://gomoku.epitest.eu

---

## 🛠️ Development Environment

### Required Tools
- Linux development environment
- Make
- Piskvork game manager (for testing)
- Git (for collaboration)

### Recommended
- Debugger (gdb, lldb)
- Profiler (gprof, valgrind)
- Unit testing framework

---

## 📚 Resources

- **Protocol Specification:** Check Epitech Mirror for piskvork protocol
- **Game Rules:** Freestyle Gomoku (5 in a row wins)
- **AI Algorithms:** Minimax, Alpha-Beta Pruning, Monte Carlo Tree Search
- **Testing Tool:** piskvork game manager

---

## ✅ Success Criteria

- ✅ Bot compiles successfully on Linux
- ✅ Follows piskvork protocol correctly
- ✅ Makes valid moves consistently
- ✅ Responds within 5 seconds per move
- ✅ Uses less than 70 MB memory
- ✅ Wins against basic/medium level bots
- ✅ Code is clean, documented, and maintainable

---

**Good luck and happy coding! 🎮🤖**
