SRC=src
OUT=target

all:
	mkdir -p $(OUT)
	gcc -Ofast -march=native -std=c99 -fgnu89-inline -fopenmp -o $(OUT)/main $(SRC)/*.c -lGL -lGLU -lglut -lm

clean:
	rm $(OUT)/*
