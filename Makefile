SRC=src
OUT=target

all:
	mkdir -p $(OUT)
	gcc -Ofast -o $(OUT)/main $(SRC)/*.c -lGL -lGLU -lglut

clean:
	rm $(OUT)/*