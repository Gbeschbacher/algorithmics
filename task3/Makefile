PROG=main
FILES=main.cpp node.cpp ukkonen.cpp

CONFIG = config.p
RESULT = results.png
DATA = main.dat


CXX = g++-4.9 --std=c++14

all: clean build run

build: $(PROG)

$(PROG): $(FILES)
	$(CXX) -O3 -o $(PROG) $(FILES)

pedantic: $(FILES)
	$(CXX) -Wextra -Werror -Weffc++ -pedantic $(FILES)

plot:
	gnuplot $(CONFIG) > $(RESULT)

show:
	shotwell $(RESULT)

run: $(PROG)
	./$(PROG)

clean:
	rm -f ./$(PROG)