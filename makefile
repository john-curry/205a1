CC=clang++ 
FLAGS=-std=c++11 -Wall -pedantic -g
OUT=bvg_draw
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

$(OUT): $(OBJ)
	$(CC) $(FLAGS) -o $@ $? && ctags *.cpp

$(OBJ): $(SRC)
	$(CC) $(FLAGS) -c $?

test: $(OUT)
	./$(OUT) test2.txt out.png && xdg-open out.png

clean: 
	rm $(OUT) $(OBJ)
