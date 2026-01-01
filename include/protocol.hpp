#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <iostream>
#include <string>
#include <vector>
#include "board.hpp"
#include "ai.hpp"

class ProtocolHandler {
private:
    // Private members for communication handling

public:
    // Constructor
    ProtocolHandler();

    // Main communication loop
    void runCommunicationLoop(Board& board, AI& ai);

    // Command handlers
    void handleStart(int boardSize);
    void handleBegin();
    void handleTurn(const std::string& command);
    void handleBoard(const std::string& command);
    void handleInfo(const std::string& command);
    void handleAbout();
    void handleEnd();
    void handleRestart();

    // Response functions
    void sendMove(int x, int y);
    void sendMessage(const std::string& message);
};

#endif // PROTOCOL_HPP
