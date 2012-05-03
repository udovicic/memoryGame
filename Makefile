SRCS = src/main.cpp src/game.cpp
HEADS = src/game.h
OBJS = main.o game.o
TRGT = memoryGame

CC = g++
CFLAGS = -g -pedantic -Wall `sdl-config --cflags`
LFLAGS = -o $(TRGT) -lSDL_image -lSDL

$(TRGT): $(OBJS)
	$(CC) $(OBJS) $(LFLAGS)

$(OBJS): $(SRCS) $(HEADS)
	$(CC) $(CFLAGS) -c $(SRCS)

clean mrproper:
	rm $(OBJS) $(TRGT)