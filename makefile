COMPILE_FLAGS = -c
ALL_FLAGS = -O2 -I.

all: prog

prog: main.o deck.o hand.o shoe.o player.o strategy.o
	g++ ${ALL_FLAGS} -o main -g main.o deck.o hand.o shoe.o player.o strategy.o

test: test.o deck.o hand.o shoe.o player.o strategy.o settings.hpp
	g++ ${ALL_FLAGS} -o test -g test.o deck.o hand.o shoe.o player.o strategy.o

test.o: test.cpp settings.hpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} test.cpp

main.o: main.cpp settings.hpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} main.cpp

deck.o: deck.cpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} deck.cpp

hand.o: hand.cpp settings.hpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} hand.cpp

shoe.o: shoe.cpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} shoe.cpp

player.o: player.cpp settings.hpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} player.cpp

strategy.o: strategy.cpp settings.hpp
	g++ ${ALL_FLAGS} ${COMPILE_FLAGS} strategy.cpp

clean:
	rm -rf *.o
	rm -rf main.exe
	rm -rf main.exe.stackdump