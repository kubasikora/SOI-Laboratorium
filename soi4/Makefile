CC = g++

all: main.o messagebuffer.o 
	$(CC) main.o messagebuffer.o -o monitor -pthread

main.o: main.cpp
	$(CC) -c main.cpp -o main.o -pthread

messagebuffer.o: messagebuffer.cpp messagebuffer.hpp monitor.hpp
	$(CC) -c messagebuffer.cpp -o messagebuffer.o -pthread

clean:
	rm -rf *.o monitor