#ifndef __LOCK_H__
#define __LOCK_H__

#include <iostream>       // std::cout
#include <atomic>         // std::atomic_flag
#include <thread>         // std::thread
#include <vector>         // std::vector
#include <sstream>        // std::stringstream

using namespace std;

void tas_lock ();
void tas_unlock ();

void ttas_lock();
void ttas_unlock ();

void ticket_lock ();
void ticket_unlock ();

void pthread_lock ();
void pthread_unlock ();

void pthread_barrier ();


#endif