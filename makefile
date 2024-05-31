CFLAGS = -c -O2

all: prog

prog: main.o deck.o hand.o shoe.o player.o
	g++ -O2 -o main -g main.o deck.o hand.o shoe.o player.o -I.

main.o: main.cpp
	g++ ${CFLAGS} main.cpp -I.

deck.o: deck.cpp
	g++ ${CFLAGS} deck.cpp -I.

hand.o: hand.cpp
	g++ ${CFLAGS} hand.cpp -I.

shoe.o: shoe.cpp
	g++ ${CFLAGS} shoe.cpp -I.

player.o: player.cpp
	g++ ${CFLAGS} player.cpp -I.

clean:
	rm -rf *.o
	rm -rf main.exe