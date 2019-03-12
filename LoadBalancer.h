#ifndef _ZMQ_Load_Balancer_LoadBalancer_h_
#define _ZMQ_Load_Balancer_LoadBalancer_h_

#include <queue>
#include <vector>
#include <Core/Core.h>
#include <ZMQ_431/ZMQ_431.h>

struct WorkerClient{
	Upp::String workerId;
	Upp::String clientId;
	
	WorkerClient(){}
	WorkerClient(Upp::String worker, Upp::String client){
		workerId = worker;
		clientId = client;
	}
};

class TrSod{
	
	
};

class TRMain{
public:
	void DoWork();
private:
	void genereteTrSod();
	
	Vector<TrSod*> sods;
};

class ZMQ_Load_Balancer{
public:
	ZMQ_Load_Balancer();
	
	void BindFront(Upp::String bindStr);
	void BindBack(Upp::String bindStr);
	void Bind(Upp::String frontStr, Upp::String backStr);
	void DoWork();
	
	
private:
	zmq::context_t context;
	zmq::socket_t frontend;
	zmq::socket_t backend;
	std::vector<zmq::pollitem_t> items;
	
	int clientCount = 0;
	int workerCount = 0;
	
	bool isReadyForWork();
	bool isPollSetInit = false;
	bool isFrontBind = false;
	bool isBackBind = false;
	
	int RecognizeClient(Upp::String clientAddr);
	int RecognizeWorker(Upp::String workerAddr);
	
	void SetWorkerClient(Upp::String clientAddr);
	void DeleteWorkerClient(Upp::String clientAddr);
	
	void Coordinate();
	
	Upp::Vector<WorkerClient*> vWorkerClient;
	
	std::queue<Upp::String> workerQueue;
	std::queue<Upp::String> clientQueue;
};

#endif