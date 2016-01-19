CC=clang++ 

all: 
	clang++ -std=c++11 -Wall -pedantic -o bvg_draw *.cpp

test: bvg_draw
	./bvg_draw test2.txt out.png && xdg-open out.png


