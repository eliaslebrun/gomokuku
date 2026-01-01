#include "protocol.hpp"
#include "utils.hpp"
#include <sstream>

// Global state to track our color
static Cell myColor = Cell::BLACK;
static Board* globalBoard = nullptr;
static AI* globalAI = nullptr;

ProtocolHandler::ProtocolHandler() {
    // Constructor implementation
}

void ProtocolHandler::runCommunicationLoop(Board& board, AI& ai) {
    globalBoard = &board;
    globalAI = &ai;
    
    std::string line;
    
    while (std::getline(std::cin, line)) {
        line = trimString(line);
        
        if (line.empty()) {
            continue;
        }
        
        // Parse command
        if (line.find("START") == 0) {
            auto parts = splitString(line, ' ');
            if (parts.size() >= 2) {
                int size = std::stoi(parts[1]);
                handleStart(size);
            } else {
                handleStart(20); // Default size
            }
        }
        else if (line == "BEGIN") {
            myColor = Cell::BLACK;
            handleBegin();
        }
        else if (line.find("TURN") == 0) {
            handleTurn(line);
        }
        else if (line == "BOARD") {
            handleBoard(line);
        }
        else if (line.find("INFO") == 0) {
            handleInfo(line);
        }
        else if (line == "ABOUT") {
            handleAbout();
        }
        else if (line == "END") {
            handleEnd();
            return; // Exit the loop
        }
        else if (line == "RESTART") {
            handleRestart();
        }
    }
}

void ProtocolHandler::handleStart(int boardSize) {
    if (globalBoard) {
        globalBoard->clear();
    }
    sendMessage("OK");
}

void ProtocolHandler::handleBegin() {
    // We play first (BLACK)
    // Play in the center
    int centerX = 10;
    int centerY = 10;
    
    if (globalBoard) {
        globalBoard->placeStone(centerX, centerY, myColor);
    }
    
    sendMove(centerX, centerY);
}

void ProtocolHandler::handleTurn(const std::string& command) {
    // Parse "TURN x,y"
    auto parts = splitString(command, ' ');
    if (parts.size() < 2) {
        return;
    }
    
    auto coords = splitString(parts[1], ',');
    if (coords.size() < 2) {
        return;
    }
    
    int opponentX = std::stoi(coords[0]);
    int opponentY = std::stoi(coords[1]);
    
    // Opponent color is opposite of ours
    Cell opponentColor = (myColor == Cell::BLACK) ? Cell::WHITE : Cell::BLACK;
    
    if (globalBoard) {
        globalBoard->placeStone(opponentX, opponentY, opponentColor);
        
        // Now we need to play
        Move bestMove = globalAI->findBestMove(*globalBoard, myColor);
        
        // Check if move is valid
        if (globalBoard->isValidMove(bestMove.first, bestMove.second)) {
            globalBoard->placeStone(bestMove.first, bestMove.second, myColor);
            sendMove(bestMove.first, bestMove.second);
        } else {
            // Fallback: find any valid move
            auto availableMoves = globalBoard->getAvailableMoves();
            if (!availableMoves.empty()) {
                Move fallback = availableMoves[0];
                globalBoard->placeStone(fallback.first, fallback.second, myColor);
                sendMove(fallback.first, fallback.second);
            }
        }
    }
}

void ProtocolHandler::handleBoard(const std::string& command) {
    // Clear board first
    if (globalBoard) {
        globalBoard->clear();
    }
    
    // Read board state line by line until "DONE"
    std::string line;
    while (std::getline(std::cin, line)) {
        line = trimString(line);
        
        if (line == "DONE") {
            break;
        }
        
        // Parse "x,y,player"
        auto parts = splitString(line, ',');
        if (parts.size() >= 3) {
            int x = std::stoi(parts[0]);
            int y = std::stoi(parts[1]);
            int player = std::stoi(parts[2]);
            
            Cell stone = (player == 1) ? Cell::BLACK : Cell::WHITE;
            
            if (globalBoard) {
                globalBoard->placeStone(x, y, stone);
            }
        }
    }
    
    // Determine our color based on move count
    if (globalBoard) {
        int moveCount = globalBoard->getMoveCount();
        // If odd number of moves, we are WHITE, otherwise BLACK
        myColor = (moveCount % 2 == 0) ? Cell::BLACK : Cell::WHITE;
        
        // Now make our move
        Move bestMove = globalAI->findBestMove(*globalBoard, myColor);
        
        if (globalBoard->isValidMove(bestMove.first, bestMove.second)) {
            globalBoard->placeStone(bestMove.first, bestMove.second, myColor);
            sendMove(bestMove.first, bestMove.second);
        } else {
            // Fallback
            auto availableMoves = globalBoard->getAvailableMoves();
            if (!availableMoves.empty()) {
                Move fallback = availableMoves[0];
                globalBoard->placeStone(fallback.first, fallback.second, myColor);
                sendMove(fallback.first, fallback.second);
            }
        }
    }
}

void ProtocolHandler::handleInfo(const std::string& command) {
    // INFO commands are optional, we can ignore them
    // Or respond with appropriate values
}

void ProtocolHandler::handleAbout() {
    sendMessage("name=\"GomokuAI\", version=\"1.0\", author=\"YourTeam\"");
}

void ProtocolHandler::handleEnd() {
    // Clean exit
    std::exit(0);
}

void ProtocolHandler::handleRestart() {
    if (globalBoard) {
        globalBoard->clear();
    }
    sendMessage("OK");
}

void ProtocolHandler::sendMove(int x, int y) {
    std::cout << x << "," << y << std::endl;
    std::cout.flush(); // CRITICAL: flush stdout
}

void ProtocolHandler::sendMessage(const std::string& message) {
    std::cout << message << std::endl;
    std::cout.flush(); // CRITICAL: flush stdout
}