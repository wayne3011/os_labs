SRC=$(wildcard *.c helpers/*.c)

OBJ=$(patsubst %.c,build/%.o,$(SRC))
CFLAGS=-std=c11
CC=gcc

PROJECT=sobel_os_lab

all: install

install: dir clang-format $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o build/$(PROJECT) -ljpeg -lm

build/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@


clang-format:
	clang-format -i *.c helpers/*.c

clean:
	rm -rf report/
	rm -rf build/*

commit:
	git fetch
	git pull
	git add ..
	git commit -m "update"

dir:
	mkdir -p build/helpers
