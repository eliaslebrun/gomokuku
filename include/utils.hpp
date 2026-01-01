#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <sstream>

// String processing utilities
std::vector<std::string> splitString(const std::string& str, char delimiter);
std::string trimString(const std::string& str);

// Debug and logging
void logMessage(const std::string& message);
void setDebugMode(bool enabled);

// Timing utilities
unsigned long long getCurrentTimeMs();

// Random number generation (for tie-breaking)
int getRandomInt(int min, int max);

// Pattern matching helpers
bool patternMatch(const std::string& text, const std::string& pattern);

// Validation helpers
bool isValidCoordinate(int x, int y);
bool isValidBoardSize(int size);

// Platform-specific helpers
std::string getOSName();

#endif // UTILS_HPP
