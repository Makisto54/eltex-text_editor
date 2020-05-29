.PHONY: clean

all: text

text: text.c
	gcc text.c -o text -lncurses

clean:
	rm -rf text