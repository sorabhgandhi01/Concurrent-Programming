all: file tree test

file: generate_random_num.c
	g++ -Wall -Werror -g -I. -o file generate_random_num.c -pthread

tree: main.c helper.c fine_lock_bst.c rw_lock_bst.c
	g++ -Wall -I. -o tree main.c helper.c fine_lock_bst.c rw_lock_bst.c -pthread

test: unit_test.c helper.c fine_lock_bst.c
	g++ -Wall -I. -o test unit_test.c helper.c fine_lock_bst.c -pthread

clean:
	rm -f file tree test *.o