CFLAGS = -std=c11 -Wall -Werror -Wextra

OS := $(shell uname)
ifeq ($(OS),Linux)
	CHECK_LIB = -lcheck -lsubunit 
endif

ifeq ($(OS),Darwin)
	CHECK_LIB = -lcheck 
endif

all: clean gcov_report
	
gcc:
	gcc -D MAIN *.c
	./a.out

s21_matrix.a: 
	gcc $(CFLAGS) -c *.c
	ar rc s21_matrix.a *o 
	ranlib s21_matrix.a

test: clean s21_matrix.a
	gcc -D TEST $(CFLAGS) -c tests/*.c $(CHECK_LIB) -lm
	gcc *.o s21_matrix.a -o s21_test $(CHECK_LIB) -lm
	./s21_test

clang-format:
	clang-format -n -style=Google tests/*.c *.c *.h
	clang-format -i -style=Google tests/*.c *.c *.h

gcov_report: 
	gcc --coverage $(CFLAGS) -c -D TEST tests/*.c *.c $(CHECK_LIB) -lm
	gcc --coverage *.o -o test_g $(CHECK_LIB) -lm
	./test_g
	lcov -t "test_g" -o test_g.info -c -d .
	genhtml -o report/ test_g.info
	open ./report/index.html

clean:
	rm -rf *o *a s21_test a.out test_g report