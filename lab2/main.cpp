#include <iostream>       // std::cout
#include <atomic>         // std::atomic_flag
#include <thread>         // std::thread
#include <vector>         // std::vector
#include <sstream>        // std::stringstream

#include "lock.h"

atomic<bool> lock = ATOMIC_FLAG_INIT;

atomic<int> next_num = 0;
atomic<int> now_serving = 0;

int main (int argc, char **argv)
{
	ttas_lock (&lock);
	printf("Locked\n");
	ttas_lock (&lock);
	printf("Locked\n");
	ttas_unlock (&lock);
	printf("UnLocked\n");

	return 0;
}