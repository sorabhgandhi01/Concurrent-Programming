#include "lock.h"

atomic<bool> lock;
//atomic<bool> tas_lock;
atomic<int> next_num (0); 
atomic<int> now_serving (0);

pthread_mutex_t mutex_lock;
pthread_barrier_t pth_barr;


void tas_lock ()
{
	bool expected, changed;
    do{
        expected = false;
        changed = true;
    }while(!lock.compare_exchange_strong(expected,changed));
}

void tas_unlock ()
{
	lock.store(false);
}

void ttas_lock ()
{
    do {
    	while (lock.load()) continue;
    } while (lock.exchange(true)); // actual atomic locking
    return;
}

void ttas_unlock ()
{
	lock.store(false);
}

void ticket_lock () 
{
	int my_num = next_num.fetch_add(1);
	while (now_serving.load() != my_num) { }
}

void ticket_unlock () 
{
	now_serving.fetch_add(1);
}

void pthread_lock ()
{
	pthread_mutex_lock(&mutex_lock);
}

void pthread_unlock ()
{
	pthread_mutex_unlock(&mutex_lock);
}

void pthread_barrier ()
{
	pthread_barrier_wait(&pth_barr);
}