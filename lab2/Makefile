all: counter

counter: counter_main.cpp helper.c lock.cpp
	g++ -Wall -Werror -I. -o counter counter_main.cpp helper.c lock.cpp -pthread

clean:
	rm -f counter *.o
