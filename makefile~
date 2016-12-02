CC = gcc -Wall -std=gnu99 -c -lm -lpthread -lrt -g
LIBS=-lSDL2

all: functions.o gfx.o threads.o timing.o gameoflife.o
	gcc $^ -o gameoflife -lm -lpthread -lrt $(LIBS)
	rm -rf *.o
	rm -rf Libs/*.gch

functions.o: Libs/functions.c Libs/functions.h
	$(CC) $^

gfx.o: Libs/gfx.c Libs/gfx.h
	$(CC) $< -c

threads.o: Libs/threads.c Libs/threads.h
	$(CC) $^

timing.o: Libs/timing.c Libs/timing.h
	$(CC) $^

gameoflife.o: gameoflife.c Libs/functions.h Libs/gfx.h Libs/threads.h Libs/timing.h
	$(CC) $^

clean:
	rm -f *.o gameoflife
	rm -rf Libs/*.gch
