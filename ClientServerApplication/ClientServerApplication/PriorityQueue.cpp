#include "PriorityQueue.h"

using std::vector;
using std::list;
using std::mutex;
using std::lock_guard;
using std::shared_ptr;
using std::condition_variable;
using std::mutex;
using std::unique_lock;

PriorityQueue::PriorityQueue(shared_ptr<condition_variable> cvServerQueue, shared_ptr<mutex> mtxServerQueue)
	: cvServerQueue_(cvServerQueue), mtxServerQueue_(mtxServerQueue)
{
	priorityQueue_ = vector<list<PTDATA>>(256, list<PTDATA>());
	count_ = 0;
}

PriorityQueue::~PriorityQueue()
{
}

void PriorityQueue::push(const PTDATA req)
{
	lock_guard<mutex> lckg(mtx_);
	BYTE index = req->cPriority;
	priorityQueue_[index].push_back(req);
	count_++;
	unique_lock<mutex> ulck(*mtxServerQueue_);
	cvServerQueue_->notify_one();
}

const PTDATA PriorityQueue::pop()
{
	lock_guard<mutex> lckg(mtx_);
	for (auto& lst : priorityQueue_) {
		if (!lst.empty()) {
			PTDATA req = lst.front();
			lst.pop_front();
			count_--;
			return req;
		}
	}
	return nullptr;
}

bool PriorityQueue::isEmpty()
{
	return count_ == 0;
}