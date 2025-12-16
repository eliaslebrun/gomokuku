# Makefile for Gomoku AI - pbrain-gomoku-ai
# Standard compilation with g++, supports all, clean, fclean, re

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -O3 -Wall -Wextra -I include/ -I src/
LDFLAGS =

# Target binary
TARGET = pbrain-gomoku-ai

# Directories
SRCDIR = src
HDRDIR = include
OBJDIR = obj

# Source files
SRCS = $(wildcard $(SRCDIR)/*.cpp)
HEADERS = $(wildcard $(HDRDIR)/*.hpp)

# Object files
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Build the binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -rf $(OBJDIR)

# Clean everything
fclean: clean
	rm -f $(TARGET)

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re

# Debug target (optional)
debug: CXXFLAGS += -g -DDEBUG
debug: re
