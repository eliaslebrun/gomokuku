/*
** Protocol Handler Implementation
** Piskvork protocol command processing
*/

#include "protocol.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>

ProtocolHandler::ProtocolHandler() {
    // Constructor implementation
}

void ProtocolHandler::runCommunicationLoop(Board& board, AI& ai) {
    std::string line;
    bool gameStarted = false;

    while (std::getline(std::cin, line)) {
        // Remove trailing whitespace
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;

        // Parse command and arguments
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        // Convert to uppercase for case-insensitive comparison
        for (char& c : command) c = std::toupper(c);

        try {
            if (command == "START") {
                int size;
                if (!(iss >> size)) {
                    sendMessage("ERROR invalid board size");
                    continue;
                }
                if (size < 5 || size > 100) {
                    sendMessage("ERROR board size must be between 5 and 100");
                    continue;
                }
                handleStart(size);
                gameStarted = true;
            } else if (command == "TURN") {
                if (!gameStarted) {
                    sendMessage("ERROR game not started");
                    continue;
                }
                std::string coords;
                if (!std::getline(iss >> std::ws, coords)) {
                    sendMessage("ERROR invalid turn coordinates");
                    continue;
                }
                handleTurn(coords);
            } else if (command == "BEGIN") {
                if (!gameStarted) {
                    sendMessage("ERROR game not started");
                    continue;
                }
                handleBegin();
            } else if (command == "BOARD") {
                if (!gameStarted) {
                    sendMessage("ERROR game not started");
                    continue;
                }
                std::string boardData;
                std::string boardLine;
                // Read DONE line to know when board data ends
                while (std::getline(std::cin, boardLine)) {
                    boardLine.erase(boardLine.find_last_not_of(" \t\r\n") + 1);
                    if (boardLine == "DONE") break;
                    if (!boardData.empty()) boardData += "\n";
                    boardData += boardLine;
                }
                handleBoard(boardData);
            } else if (command == "INFO") {
                std::string key, value;
                if (!(iss >> key)) {
                    sendMessage("ERROR invalid info format");
                    continue;
                }
                std::getline(iss >> std::ws, value);
                handleInfo(key + " " + value);
            } else if (command == "ABOUT") {
                handleAbout();
            } else if (command == "END") {
                handleEnd();
                break; // Exit loop
            } else if (command == "RESTART") {
                handleRestart();
                gameStarted = false;
            } else {
                sendMessage("UNKNOWN " + command);
            }
        } catch (const std::exception& e) {
            sendMessage("ERROR " + std::string(e.what()));
        }
    }
}

void ProtocolHandler::handleStart(int boardSize) {
    // Note: Our Board class is fixed at 20x20, but protocol allows other sizes
    // For now, we only support 20x20 as per project requirements
    if (boardSize != 20) {
        sendMessage("ERROR unsupported board size, only 20x20 supported");
        return;
    }
    sendMessage("OK");
}

void ProtocolHandler::handleBegin() {
    // Make first move as black player (center of board)
    sendMove(10, 10);
}

void ProtocolHandler::handleTurn(const std::string& coords) {
    // Parse coordinates "x,y"
    size_t commaPos = coords.find(',');
    if (commaPos == std::string::npos) {
        sendMessage("ERROR invalid coordinate format, expected x,y");
        return;
    }

    try {
        int x = std::stoi(coords.substr(0, commaPos));
        int y = std::stoi(coords.substr(commaPos + 1));

        // Validate coordinates
        if (x < 0 || x >= 20 || y < 0 || y >= 20) {
            sendMessage("ERROR coordinates out of bounds");
            return;
        }

        // Note: In a full implementation, we would:
        // 1. Place opponent's stone on board
        // 2. Check if it's a valid move
        // 3. Find AI response move
        // 4. Send the move

        // For now, just acknowledge and make a simple response
        // TODO: Integrate with actual Board and AI classes
        sendMove(x + 1, y + 1); // Simple response for testing

    } catch (const std::exception&) {
        sendMessage("ERROR invalid coordinate values");
    }
}

void ProtocolHandler::handleBoard(const std::string& boardData) {
    // Parse board state from multiple lines
    // Format: each line is "x y stone" where stone is 1 (black) or 2 (white)
    std::istringstream iss(boardData);
    std::string line;

    // Note: In full implementation, we would parse and set board state
    // For now, just acknowledge and make a move
    // TODO: Implement full board parsing and AI integration

    sendMove(10, 11); // Simple response for testing
}

void ProtocolHandler::handleInfo(const std::string& command) {
    // INFO commands are optional, just acknowledge
    // Could be used for configuration like timeout, etc.
}

void ProtocolHandler::handleAbout() {
    sendMessage("name=\"Gomoku AI\", version=\"1.0\", author=\"Epitech Student\"");
}

void ProtocolHandler::handleEnd() {
    // Clean up if needed
}

void ProtocolHandler::handleRestart() {
    // Reset game state if needed
    sendMessage("OK");
}

void ProtocolHandler::sendMove(int x, int y) {
    // TODO: Send move coordinates in protocol format
    std::cout << x << "," << y << std::endl;
}

void ProtocolHandler::sendMessage(const std::string& message) {
    // TODO: Send message response
    std::cout << message << std::endl;
}
