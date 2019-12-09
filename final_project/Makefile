all: file tree

file: generate_random_num.c
	g++ -Wall -Werror -g -I. -o file generate_random_num.c -pthread

tree: main.c helper.c fine_lock_bst.c
	g++ -Wall -I. -o tree main.c helper.c fine_lock_bst.c -pthread

clean:
	rm -f file tree *.o
