#include "Client.h"
#include "PriorityQueue.h"

#include <random>
#include <iostream>
#include <thread>

using std::shared_ptr;
using std::condition_variable;
using std::mutex;
using std::atomic_int;
using std::unique_lock;

Client::Client(shared_ptr<PriorityQueue> priorityQueue, shared_ptr<condition_variable> cvClientMain, 
	shared_ptr<mutex> mtxClientMain, shared_ptr<atomic_int> counter)
	: priorityQueue_(priorityQueue), cvClientMain_(cvClientMain), mtxClientMain_(mtxClientMain), counter_(counter)
{
	dataBlock_ = new TDATA();
	dataBlock_->cPriority = rand() % 256;
	dataBlock_->dwTicks = GetTickCount();
	dataBlock_->dwClientId = rand();
}

Client::~Client()
{
}

void Client::request() const
{
	priorityQueue_->push(dataBlock_);
	unique_lock<mutex> lck(*mtxClientMain_);
	(*counter_)++;
	cvClientMain_->notify_one();
}