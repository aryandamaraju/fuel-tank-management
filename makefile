CXX = g++
CXXFLAGS = -Wall -g

proj1: fuel.o mytest.cpp
	$(CXX) $(CXXFLAGS) fuel.o mytest.cpp -o proj1

fuel.o: fuel.cpp fuel.h
	$(CXX) $(CXXFLAGS) -c fuel.cpp

run:
	./proj1

val1:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./proj1

clean:
	rm ~
	rm.o
