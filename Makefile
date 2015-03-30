FILE = fib.cpp
CONFIG = config.p
RESULT = results.png
MAIN = fib

all: build.compact run

run:
	./$(MAIN)

build: $(FILE)
	g++ --std=c++14 -O0 -Wall -Wextra -Werror -Weffc++ -o $(MAIN) $(FILE) -lgmpxx -lgmp

build.compact: $(FILE)
	$(CXX) --std=c++14 -O2 -o $(MAIN) $(FILE)

plot:
	gnuplot $(CONFIG) > $(RESULT)

clean.main:
	rm ./$(MAIN)

clean.dat:
	rm *.dat

clean: clean.dat clean.main