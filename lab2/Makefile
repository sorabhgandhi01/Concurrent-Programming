all: counter mysort

counter: counter_main.cpp helper.c lock.cpp
	g++ -Wall -Werror -g -I. -o counter counter_main.cpp helper.c lock.cpp -pthread

mysort: bucket_main.cpp helper.c lock.cpp
	g++ -Wall -Werror -I. -o mysort bucket_main.cpp helper.c lock.cpp -pthread

clean:
	rm -f counter mysort *.o
