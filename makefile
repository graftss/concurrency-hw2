build:
	gcc -O0 -Wall -Wextra -std=gnu99 -o mat-mult mat-mult.c

test: build
	./mat-mult

build-opt:
	gcc -O3 -Wall -Wextra -std=gnu99 -o mat-mult-opt mat-mult.c

test-opt: build-opt
	./mat-mult-opt
