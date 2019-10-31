all: mysort

mysort: main.c helper.c mysort.c
	cc -Wall -Werror -I. -o mysort -fopenmp main.c helper.c mysort.c

clean:
	rm -f mysort *.o
