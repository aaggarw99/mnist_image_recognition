CFLAGS = -std=c11 -pedantic -Wall -Werror -g
CC = gcc


all: test_mnist test_distance test_knn ocr
	make test_mnist
	make test_distance
	make test_knn
	make ocr

test_mnist: test_mnist.c mnist.h mnist.c
	$(CC) $(CFLAGS) -o test_mnist  $(filter %.c,$^) -lcunit -O3 -lm

test_distance: test_distance.c distance.h distance.c mnist.h mnist.c
	$(CC) $(CFLAGS) -o  test_distance $(filter %.c,$^) -lcunit -O3 -lm

test_knn: test_knn.c knn.h knn.c distance.h distance.c mnist.h mnist.c
	$(CC) $(CFLAGS) -o test_knn $(filter %.c,$^) -lcunit -O3 -lm

ocr: ocr.c mnist.h mnist.c distance.h distance.c knn.h knn.c
	$(CC) $(CFLAGS) -o ocr $(filter %.c,$^) -lcunit -O3 -lm

.PHONY: clean test distance knn o
valgrind_mnist: test_mnist
	make test_mnist
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./test_mnist

valgrind_distance: test_distance
	make test_distance
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./test_distance

valgrind_knn: test_knn
	make test_knn
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./test_knn

valgrind_ocr: ocr
	make ocr
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./ocr

mnist: test_mnist
	make test_mnist
	./test_mnist

distance: test_distance
	make test_distance
	./test_distance

knn: test_knn
	make test_knn
	./test_knn

o: ocr
	make ocr
	./ocr

clean:
	-rm test_mnist test_distance test_knn ocr




