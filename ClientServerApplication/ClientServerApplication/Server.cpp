#include "Server.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>

using std::ofstream;
using std::endl;
using std::stringstream;
using std::cout;
using std::shared_ptr;
using std::condition_variable;
using std::mutex;
using std::string;
using std::ios_base;
using std::unique_lock;

Server::Server(shared_ptr<PriorityQueue> priorityQueue, shared_ptr<condition_variable> cvServerQueue, 
	shared_ptr<mutex> mtxServerQueue, shared_ptr<condition_variable> cvServerMain, shared_ptr<mutex> mtxServerMain)
	: priorityQueue_(priorityQueue), cvServerQueue_(cvServerQueue), mtxServerQueue_(mtxServerQueue)
	, cvServerMain_(cvServerMain), mtxServerMain_(mtxServerMain)
{
	stop_ = false;
}

Server::~Server()
{
}

void Server::run()
{
	while (!stop_ || !priorityQueue_->isEmpty()) {
		unique_lock<mutex> lckServerQueue(*mtxServerQueue_);
		while (priorityQueue_->isEmpty()) {
			cvServerQueue_->wait(lckServerQueue);
		}
		if (!priorityQueue_->isEmpty()) {
			receive();
		}
	}

	unique_lock<mutex> lckServerMain(*mtxServerMain_);
	cvServerMain_->notify_one();
}

void Server::receive()
{
	PTDATA data = priorityQueue_->pop();
	if (data != nullptr) {
		write(data);
		delete data;
	}
}

void Server::write(PTDATA data)
{
	const string name = "log.txt";

	ios_base::openmode mode;
	if (!firstWrite_) {
		mode = ios_base::app;
	}
	else {
		mode = ios_base::out;
	}

	ofstream fout;

	fout.open(name, mode);

	if (fout.is_open()) {
		if (firstWrite_) {
			fout << "write time, client id, priority, request time" << endl;
			firstWrite_ = false;
		}

		stringstream sstr;
		sstr << GetTickCount() << ", " << data->dwClientId << ", "
			<< static_cast<unsigned int>(data->cPriority) << ", " << data->dwTicks;
		fout << sstr.str() << endl;

		fout.close();
	}
	else {
		cout << "Cannot open '" << name << "' file to write!" << endl;
	}
}

void Server::stop()
{
	unique_lock<mutex> lck(*mtxServerQueue_);
	stop_ = true;
	cvServerQueue_->notify_all();
}