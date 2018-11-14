a.out : main.o
	g++ -std=c++14 -o a.out main.o


main.o : main.cpp main.h FileStream.h
	g++ -std=c++14 -c main.cpp

clean : 
	rm a.out main.o