#pragma once

#include "PriorityQueue.h"

class Server
{
public:
	Server(std::shared_ptr<PriorityQueue> priorityQueue, 
		std::shared_ptr<std::condition_variable> cvServerQueue, std::shared_ptr<std::mutex> mtxServerQueue,
		std::shared_ptr<std::condition_variable> cvServerMain, std::shared_ptr<std::mutex> mtxServerMain);
	~Server();
	void run();
	void receive();
	void write(PTDATA data);
	void stop();
private:
	std::shared_ptr<PriorityQueue> priorityQueue_;
	bool firstWrite_ = true;
	std::atomic_bool stop_;
	std::shared_ptr<std::condition_variable> cvServerQueue_;
	std::shared_ptr<std::mutex> mtxServerQueue_;
	std::shared_ptr<std::condition_variable> cvServerMain_;
	std::shared_ptr<std::mutex> mtxServerMain_;
};