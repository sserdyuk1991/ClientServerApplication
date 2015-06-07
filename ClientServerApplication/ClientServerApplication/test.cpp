#include "Client.h"
#include "Server.h"
#include "PriorityQueue.h"

#include <thread>
#include <iostream>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::thread;
using std::condition_variable;
using std::unique_lock;
using std::mutex;
using std::cout;
using std::endl;
using std::atomic_int;
using std::cin;
using std::chrono::seconds;

int main()
{
	int n;
	cout << "Please enter the number of clients: ";
	cin >> n;

	srand(time(0));

	shared_ptr<condition_variable> cvClientMain = make_shared<condition_variable>();
	shared_ptr<mutex> mtxClientMain = make_shared<mutex>();

	shared_ptr<condition_variable> cvServerQueue = make_shared<condition_variable>();
	shared_ptr<mutex> mtxServerQueue = make_shared<mutex>();

	shared_ptr<condition_variable> cvServerMain = make_shared<condition_variable>();
	shared_ptr<mutex> mtxServerMain = make_shared<mutex>();
	
	shared_ptr<PriorityQueue> priorityQueue = make_shared<PriorityQueue>(cvServerQueue, mtxServerQueue);
	
	shared_ptr<atomic_int> counter = make_shared<atomic_int>();
	*counter = 0;

	vector<unique_ptr<Client>> clients;
	for (int i = 0; i < n; i++) {
		clients.emplace_back(new Client(priorityQueue, cvClientMain, mtxClientMain, counter));
		thread thd(&Client::request, *clients.back());
		thd.detach();
	}

	Server s(priorityQueue, cvServerQueue, mtxServerQueue, cvServerMain, mtxServerMain);
	thread thd(&Server::run, s);
	thd.detach();

	unique_lock<mutex> lckClientMain(*mtxClientMain);
	while (*counter < n) {
		cvClientMain->wait(lckClientMain);
	}

	s.stop();

	unique_lock<mutex> lckServerMain(*mtxServerMain);
	cvServerMain->wait_for(lckServerMain, seconds(1));

	cout << "Log-file was created!" << endl;
	system("pause");
	return 0;
}