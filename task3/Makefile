PROG=main
FILES=main.cpp node.cpp ukkonen.cpp

CXX1 = g++-4.9 -Wall --std=c++14
CXX = g++-4.9 --std=c++14

all: clean build run

build: $(PROG)

$(PROG): $(FILES)
	$(CXX) -O3 -o $(PROG) $(FILES)

pedantic: $(FILES)
	$(CXX) -Wextra -Werror -Weffc++ -pedantic $(FILES)

time: $(PROG)
	time ./$(PROG)

cg: $(PROG)
	valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes  --simulate-cache=yes ./$(PROG)

run: $(PROG)
	./$(PROG)

clean:
	rm -f ./$(PROG)