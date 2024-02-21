
CC = g++
CFLAGS = -Wall -std=c++17

# Executable name
EXEC = game.exe

SRCS = game.cpp statsmaker.cpp team.cpp
OBJS = $(SRCS:.cpp=.o)
HDRS = game.h player.h team.h
# Default rule
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to build object files
%.o: %.cpp $(HDRS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJS)
