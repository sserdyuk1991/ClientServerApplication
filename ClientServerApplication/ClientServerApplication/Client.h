#pragma once

#include <windows.h>
#include <memory>
#include <condition_variable>
#include <atomic>

typedef struct tagTDATA {
	BYTE cPriority;	// ѕриоритет запроса 0 Ц 255 (0 Ц наивысший приоритет).
	DWORD dwTicks; // ¬рем€ формировани€ запроса в системных тиках.
	DWORD dwClientId; // ”никальный идентификатор клиента.
	char Data[255];	// јбстрактные данные.
} TDATA, *PTDATA;

class PriorityQueue;

class Client
{
public:
	Client(std::shared_ptr<PriorityQueue> priorityQueue, std::shared_ptr<std::condition_variable> cvClientMain,
		std::shared_ptr<std::mutex> mtxClienMain, std::shared_ptr<std::atomic_int> counter);
	~Client();
	void request() const;
private:
	PTDATA dataBlock_;
	std::shared_ptr<PriorityQueue> priorityQueue_;
	std::shared_ptr<std::condition_variable> cvClientMain_;
	std::shared_ptr<std::mutex> mtxClientMain_;
	std::shared_ptr<std::atomic_int> counter_;
};