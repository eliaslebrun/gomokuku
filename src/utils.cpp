/*
** Utility Functions Implementation
** Helper functions for string processing, debug logging, etc.
*/

#include "utils.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <cstdlib>

// String processing utilities
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string trimString(const std::string& str) {
    // TODO: Remove leading/trailing whitespace
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

// Debug and logging
static bool debugEnabled = false;

void logMessage(const std::string& message) {
    // TODO: Only log when debug is enabled
    if (debugEnabled) {
        std::cerr << "[DEBUG] " << message << std::endl;
    }
}

void setDebugMode(bool enabled) {
    debugEnabled = enabled;
}

// Timing utilities
unsigned long long getCurrentTimeMs() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

// Random number generation (for tie-breaking)
int getRandomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

// Pattern matching helpers
bool patternMatch(const std::string& text, const std::string& pattern) {
    // TODO: Simple pattern matching (could use regex)
    return text.find(pattern) != std::string::npos;
}

// Validation helpers
bool isValidCoordinate(int x, int y) {
    // TODO: Check coordinate bounds (0-19 for 20x20 board)
    return x >= 0 && x < 20 && y >= 0 && y < 20;
}

bool isValidBoardSize(int size) {
    // TODO: Gomoku uses 20x20 board
    return size == 20;
}

// Platform-specific helpers
std::string getOSName() {
    // TODO: Return OS name for logging
    #ifdef _WIN32
        return "Windows";
    #elif __linux__
        return "Linux";
    #elif __APPLE__
        return "macOS";
    #else
        return "Unknown";
    #endif
}
