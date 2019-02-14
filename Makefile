all: parsertestexe

parsertestexe: query.o main.o
	g++ main.o query.o -std=c++17 -g -o parsertestexe

query.o: Query.cpp Query.h
	g++ Query.cpp -g -std=c++17 -c -o query.o

main.o: main.cpp	
	g++ main.cpp -g -std=c++17 -c 

clean:
	rm -f *.o

.PHONY: all clean