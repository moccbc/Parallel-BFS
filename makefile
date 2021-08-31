CC = g++
CFLAGS = -O3 -mcx16 -march=native -DCILK -fcilkplus -std=c++17

all:	bfs

bfs:	bfs.cpp
	$(CC) $(CFLAGS) -DNDEBUG bfs.cpp -o bfs
	
gen:	tests/gen_graph.cpp
	$(CC) $(CFLAGS) -DNDEBUG tests/gen_graph.cpp -o gen 
	
scan:	tests/scan.cpp
	$(CC) $(CFLAGS) -DNDEBUG tests/scan.cpp -o scan

filter:	tests/filter.cpp
	$(CC) $(CFLAGS) -DNDEBUG tests/filter.cpp -o filter 
	
flatten:	tests/flatten.cpp
	$(CC) $(CFLAGS) -DNDEBUG tests/flatten.cpp -o flatten

clean:
	rm -f bfs scan filter flatten gen
