build:
	gcc -O0 -Wall -Wextra -std=gnu99 -o mat-mult mat-mult.c

test: build
	./mat-mult
