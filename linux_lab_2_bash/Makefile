SRC=$(wildcard *.c)

OBJ=$(patsubst %.c,build/%.o,$(SRC))

CFLAGS=-Wall -Werror -Wextra

PROJECT=s304sh_os_labs

all: install

install: build
	sudo mv build/$(PROJECT) /usr/bin/$(PROJECT)
	echo /usr/bin/$(PROJECT) | sudo tee -a /etc/shells
	sudo chsh -s /usr/bin/$(PROJECT) $(USER)
uninstall: clean
	sudo rm -f /usr/bin/$(PROJECT)
	sudo sed -i '/\/usr\/bin\/$(PROJECT)/d' /etc/shells
	sudo chsh -s /usr/bin/bash $(USER)
build: dir clang-format $(OBJ)
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
