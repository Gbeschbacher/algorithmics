FILE = main.cpp
CONFIG = config.p
RESULT = results.png
DATA = data.dat
MAIN = main

all: build run

run:
	./$(MAIN) >> $(DATA)

build: $(FILE)
	g++ --std=c++14 -O0 -Wall -Wextra -Werror -Weffc++ -o $(MAIN) $(FILE)

plot:
	gnuplot $(CONFIG) > $(RESULT)

clean.main:
	rm ./$(MAIN) || true

clean: clean.main