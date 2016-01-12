all: 
	g++ -Wall -pedantic -o draw_bvg *.cpp

test: draw_bvg
	./draw_bvg test.txt out.png
