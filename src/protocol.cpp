#include "protocol.hpp"
#include "utils.hpp"
#include <sstream>

// Global state to track our color
static Cell myColor = Cell::BLACK;
static Board* globalBoard = nullptr;
static AI* globalAI = nullptr;
static bool gameStarted = false;

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
                try {
                    int size = std::stoi(parts[1]);
                    if (size != 20) {
                        sendMessage("ERROR unsupported board size, only 20x20 supported");
                        continue;
                    }
                    handleStart(size);
                    gameStarted = true;
                } catch (const std::exception&) {
                    sendMessage("ERROR invalid board size");
                    continue;
                }
            } else {
                sendMessage("ERROR missing board size parameter");
                continue;
            }
        }
        else if (line == "BEGIN") {
            if (!gameStarted) {
                sendMessage("ERROR game not started");
                continue;
            }
            myColor = Cell::BLACK;
            handleBegin();
        }
        else if (line.find("TURN") == 0) {
            if (!gameStarted) {
                sendMessage("ERROR game not started");
                continue;
            }
            handleTurn(line);
        }
        else if (line == "BOARD") {
            if (!gameStarted) {
                sendMessage("ERROR game not started");
                continue;
            }
            handleBoard(line);
        }
        else if (line.find("INFO") == 0) {
            auto parts = splitString(line, ' ');
            if (parts.size() < 3) {
                sendMessage("ERROR invalid info format, expected 'INFO key value'");
                continue;
            }
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
        else {
            // Handle unknown commands
            std::string command = line.substr(0, line.find(' '));
            sendMessage("UNKNOWN " + command);
        }
    }
}

void ProtocolHandler::handleStart(int) {
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
        sendMessage("ERROR invalid turn format");
        return;
    }

    auto coords = splitString(parts[1], ',');
    if (coords.size() < 2) {
        sendMessage("ERROR invalid coordinate format, expected x,y");
        return;
    }

    try {
        int opponentX = std::stoi(coords[0]);
        int opponentY = std::stoi(coords[1]);

        // Validate coordinates
        if (opponentX < 0 || opponentX >= 20 || opponentY < 0 || opponentY >= 20) {
            sendMessage("ERROR coordinates out of bounds");
            return;
        }

        // Opponent color is opposite of ours
        Cell opponentColor = (myColor == Cell::BLACK) ? Cell::WHITE : Cell::BLACK;

        if (globalBoard) {
            // Check if the position is already occupied
            if (!globalBoard->isValidMove(opponentX, opponentY)) {
                sendMessage("ERROR square occupied");
                return;
            }

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
                } else {
                    sendMessage("ERROR no valid moves available");
                }
            }
        }
    } catch (const std::exception&) {
        sendMessage("ERROR invalid coordinate values");
    }
}

void ProtocolHandler::handleBoard(const std::string&) {
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
            try {
                int x = std::stoi(parts[0]);
                int y = std::stoi(parts[1]);
                int player = std::stoi(parts[2]);

                // Validate coordinates
                if (x < 0 || x >= 20 || y < 0 || y >= 20) {
                    sendMessage("ERROR board coordinates out of bounds");
                    continue;
                }

                // Validate player value (1 = black, 2 = white)
                if (player != 1 && player != 2) {
                    sendMessage("ERROR invalid player value, must be 1 or 2");
                    continue;
                }

                Cell stone = (player == 1) ? Cell::BLACK : Cell::WHITE;

                if (globalBoard) {
                    // Check if position is already occupied
                    if (!globalBoard->isValidMove(x, y)) {
                        sendMessage("ERROR board position already occupied");
                        continue;
                    }
                    globalBoard->placeStone(x, y, stone);
                }
            } catch (const std::exception&) {
                sendMessage("ERROR invalid board data format");
                continue;
            }
        } else {
            sendMessage("ERROR invalid board line format");
            continue;
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
            } else {
                sendMessage("ERROR no valid moves available");
            }
        }
    }
}

void ProtocolHandler::handleInfo(const std::string&) {
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
    gameStarted = false;
    myColor = Cell::BLACK;
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
