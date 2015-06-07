#pragma once

#include "Client.h"

#include <vector>
#include <list>
#include <mutex>

class PriorityQueue
{
public:
	PriorityQueue(std::shared_ptr<std::condition_variable> cvServerQueue, std::shared_ptr<std::mutex> mtxServerQueue);
	~PriorityQueue();
	void push(const PTDATA req);
	const PTDATA pop();
	bool isEmpty();
private:
	std::vector<std::list<PTDATA>> priorityQueue_;
	std::mutex mtx_;	// For synchronizing push and pop operations.
	std::shared_ptr<std::condition_variable> cvServerQueue_;
	std::shared_ptr<std::mutex> mtxServerQueue_;
	int count_;
};