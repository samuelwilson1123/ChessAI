CC = gcc
CFLAGS = -Wall -Wextra

# List of object files
OBJ = chess.o moves.o tables.o util.o execute_moves.o

# Default target
all: chess

# Rule to build the executable
chess: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o chess

# Rules to build object files
chess.o: chess.c util.h moves.h
	$(CC) $(CFLAGS) -c chess.c

moves.o: moves.c moves.h
	$(CC) $(CFLAGS) -c moves.c

tables.o: tables.c tables.h util.h
	$(CC) $(CFLAGS) -c tables.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

execute_moves.o: execute_moves.c execute_moves.h tables.h moves.h
	$(CC) $(CFLAGS) -c execute_moves.c

# Clean rule
clean:
	rm -f $(OBJ) chess
