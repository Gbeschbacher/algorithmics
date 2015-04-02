FILE = fib.cpp
CONFIG = config.p
RESULT = results.png
MAIN = fib

all: build run plot

run:
	./$(MAIN)

build: $(FILE)
	g++ --std=c++14 -O0 -Wall -Wextra -Werror -Weffc++ -o $(MAIN) $(FILE) -lgmpxx -lgmp

plot:
	gnuplot $(CONFIG) > $(RESULT)

clean.main:
	rm ./$(MAIN)

clean.dat:
	rm *.dat

clean: clean.dat clean.main