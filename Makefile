CXX_FLAGS = -Wall -Wextra -g

all: prefix_code

prefix_code:
	g++ --std=c++17 $(CXX_FLAGS) main.cpp -o main.out

clean:
	rm -vf *.out
