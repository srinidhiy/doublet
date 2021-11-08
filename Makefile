CXX = g++
CPPFLAGS = -Wall -g

all: doublet

doublet: doublet.o
	$(CXX) $(CPPFLAGS) $^ -o $@

doublet.o: doublet.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	rm *.o