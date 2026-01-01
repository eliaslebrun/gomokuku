#include <iostream>
#include <string>
#include "protocol.hpp"
#include "board.hpp"
#include "ai.hpp"
#include "utils.hpp"

int main() {
    // Initialize the bot
    ProtocolHandler protocol;
    Board board;
    AI ai;

    // Start the main communication loop
    protocol.runCommunicationLoop(board, ai);

    return 0;
}
