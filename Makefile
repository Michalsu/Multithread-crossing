multithreadCrossing:
	g++ -o  multithreadCrossing multithreadCrossing.cpp -lncurses -pthread
	
clean:
	rm -f multithreadCrossing

