SRC=$(wildcard *.c)

OBJ=$(patsubst %.c,build/%.o,$(SRC))

CFLAGS=-Wall -Werror -Wextra

PROJECT=archiver_os_lab

all: install

install: dir clang-format $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o build/$(PROJECT)

build/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@


clang-format:
	clang-format -i *.c *.h

clean:
	rm -rf report/
	rm -rf build/*

commit:
	git fetch
	git pull
	git add ..
	git commit -m "update"

dir:
	mkdir -p build/
