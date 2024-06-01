COMPILE_FLAGS = -c
ALL_FLAGS = -O2 -I.


all: prog

prog: main.o deck.o hand.o shoe.o player.o
	g++ ${ALL_FLAGS} -o main -g main.o deck.o hand.o shoe.o player.o

main.o: main.cpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} main.cpp

deck.o: deck.cpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} deck.cpp

hand.o: hand.cpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} hand.cpp

shoe.o: shoe.cpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} shoe.cpp

player.o: player.cpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} player.cpp

clean:
	rm -rf *.o
	rm -rf main.exe
	rm -rf main.exe.stackdump