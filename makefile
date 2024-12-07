# I am a comment, and I want to say that the variable CC will be # the compiler to use. 

CC=g++ 

# Hey!, I am comment number 2. I want to say that CFLAGS will be the # options I'll pass to the compiler. 
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= M04

default: main.o Matrices.o
	$(CC) main.o Matrices.o -o $(EXENAME)

main.o: main.cpp Matrices.h
	$(CC) $(CFLAGS) main.cpp 

Matrices.o: Matrices.cpp Matrices.h
	$(CC) $(CFLAGS) Matrices.cpp 

clean: 
	rm *.o $(EXENAME)

run:
	./$(EXENAME)