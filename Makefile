CC=clang++
CFLAGS=-I.

test: 
	$(CC) -o test -lgtest -lgtest_main
