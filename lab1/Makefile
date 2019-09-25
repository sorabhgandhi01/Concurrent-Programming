all: mysort

mysort: main.c helper.c mysort.c
	cc -Wall -Werror -I. -o mysort main.c helper.c mysort.c -pthread

clean:
	rm -f mysort *.o
