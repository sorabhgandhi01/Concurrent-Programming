#ifndef __LOCK_H__
#define __LOCK_H__

#include <iostream>       // std::cout
#include <atomic>         // std::atomic_flag
#include <thread>         // std::thread
#include <vector>         // std::vector
#include <sstream>        // std::stringstream

using namespace std;

class Node
{
public:
	atomic<Node*> next;
	atomic<bool> wait;
};

extern atomic<Node*> tail;


class MCSLock {
public:
	void acquire(Node *myNode) {

	Node *oldTail = tail.load();
	myNode->next.store(NULL, memory_order_relaxed);
	while (!tail.compare_exchange_strong(oldTail, myNode)) {
		oldTail = tail.load();
	}

	if (oldTail != NULL) {
		myNode->wait.store(true, memory_order_relaxed);
		oldTail->next.store(myNode);
		while (myNode->wait.load()) {}
	}
	}

	void release(Node *myNode) {

		Node* temp_node = myNode;
		if (tail.compare_exchange_strong(temp_node, NULL)) {

		} else {
			while (myNode->next.load() == NULL) {}
			myNode->next.load()->wait.store(false);
		}
	}
};



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