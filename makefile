CC=clang++ 
FLAGS=-std=c++14 -Wall -pedantic -g
OUT=bvg_draw
SRC=bvg.cpp draw_bvg.cpp lodepng.cpp
OBJ=bvg.o draw_bvg.o lodepng.o

.PHONY: all clean run debug

all: bvg_draw

bvg_draw: $(SRC)
	$(CC) $(FLAGS) -o $@ $(SRC)

#$(OBJ): $(SRC)
#	$(CC) -c 

run:
	./$(OUT) test2.txt out.png && xdg-open out.png

clean: 
	rm $(OUT) $(OBJ)

debug:
	gdb --args $(OUT) test2.txt out.png
