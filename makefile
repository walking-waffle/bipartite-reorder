main:main.o Graph.o Reorder.o
	g++ -o main main.o Graph.o Reorder.o

main.o:main.cpp Graph.h Reorder.h
	g++ -c main.cpp

Graph.o:Graph.cpp Graph.h
	g++ -c Graph.cpp Graph.h

Reorder.o:Reorder.cpp Reorder.h
	g++ -c Reorder.cpp Reorder.h
clean:
	rm -f *.o *.h.gch
