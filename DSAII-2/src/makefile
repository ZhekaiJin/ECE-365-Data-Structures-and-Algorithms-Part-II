useHeap.exe: useHeap.o heap.o hash.o
	g++ -o useHeap.out useHeap.o heap.o hash.o

useHeap.o: useHeap.cpp
	g++ -c -std=c++11 useHeap.cpp

heap.o: heap.cpp heap.hpp
	g++ -c -std=c++11 heap.cpp

hash.o: hash.cpp hash.h
	g++ -c -std=c++11 hash.cpp

debug:
	g++ -g -o useHeapDebug.out useHeap.cpp heap.cpp hash.cpp

clean:
	rm -f *.out *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups