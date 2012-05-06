SRCS = src/main.cpp src/game.cpp
HEADS = src/game.h
OBJS = main.o game.o
TRGT = memoryGame

CC = g++
CFLAGS = -pedantic -Wall `sdl-config --cflags` -O2
LFLAGS = -o $(TRGT) -lSDL_image -lSDL

all: $(TRGT)

debug: CFLAGS += -DDEBUG -g
debug: $(TRGT)
	
$(TRGT): $(OBJS)
	$(CC) $(OBJS) $(LFLAGS)

$(OBJS): $(SRCS) $(HEADS)
	$(CC) $(CFLAGS) -c $(SRCS)

clean mrproper:
	rm $(OBJS) $(TRGT)