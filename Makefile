all: assembler

%: %.cc
	g++ -std=c++11 $< -o $@

%: %.cpp
	gcc $< -o $@

