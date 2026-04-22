game: main.c
	gcc main.c -o game -std=c89 -g

.PHONY: clean

clean:
	rm game
