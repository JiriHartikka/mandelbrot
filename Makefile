SRC=src
OUT=target

all:
	mkdir -p $(OUT)
	gcc -Ofast -march=native -std=c99 -fgnu89-inline -fopenmp -o $(OUT)/main $(SRC)/*.c -lGL -lGLU -lglut -lm

web:
	mkdir -p $(OUT)
	emcc -s 'EXTRA_EXPORTED_RUNTIME_METHODS=["printErr"]' -pthread -s PTHREAD_POOL_SIZE=100 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s LEGACY_GL_EMULATION -O3 -march=native -std=c99 -fgnu89-inline -fopenmp -o $(OUT)/main.html $(SRC)/*.c -lGL -lGLU -lglut -lm

clean:
	rm $(OUT)/*
