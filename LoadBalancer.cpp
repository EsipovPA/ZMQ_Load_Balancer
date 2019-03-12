#include "LoadBalancer.h"

using namespace Upp;

//========================================================================
//Useful functions
bool BindByPtr(zmq::socket_t *sockPtr, String *bindStr)
{
	try{
		sockPtr->bind(*bindStr);
	}catch (zmq::error_t err)
	{
		LOG("Error in BindByPtr");
		throw err;
		return false;
	}
	return true;
}

String S_recv(zmq::socket_t *socket)
{
	zmq::message_t msg;
	socket->recv(&msg);
	
	return String(static_cast<char*>(msg.data()), msg.size());
}

bool S_send(zmq::socket_t *socket, String msgStr, int zmqFlag = 0){
	zmq::message_t message(msgStr.GetCharCount());
	
	memcpy(message.data(), msgStr, msgStr.GetCharCount());
	
	return socket->send(message, zmqFlag);
}

//========================================================================
//ZMQ_Load_Balancer

ZMQ_Load_Balancer::ZMQ_Load_Balancer():
	context(1), frontend(context, ZMQ_ROUTER), backend(context, ZMQ_ROUTER) {
	items.push_back(zmq::pollitem_t{this->frontend, 0, ZMQ_POLLIN, 0});
	items.push_back(zmq::pollitem_t{this->backend, 0, ZMQ_POLLIN, 0});
	this->isPollSetInit = true;
}

void ZMQ_Load_Balancer::BindFront(String bindStr){
	isFrontBind = BindByPtr(&frontend, &bindStr);
}

void ZMQ_Load_Balancer::BindBack(String bindStr){
	isBackBind = BindByPtr(&backend, &bindStr);
}

void ZMQ_Load_Balancer::Bind(String frontStr, String backStr){
	BindFront(frontStr);
	BindBack(backStr);
}

bool ZMQ_Load_Balancer::isReadyForWork(){
	return isFrontBind && isBackBind && isPollSetInit;
}

int ZMQ_Load_Balancer::RecognizeClient(String clientAddr)
{
	for (int i = 0; i < vWorkerClient.GetCount(); i++){
		if (vWorkerClient[i]->clientId == vWorkerClient){
			return i;
		}
	}
	return -1;
}

int ZMQ_Load_Balancer::RecognizeWorker(String workerAddr){
	for (int i = 0; i < vWorkerClient.GetCount(); i++){
		if (vWorkerClient[i]->workerId == workerAddr){
			return i;
		}
	}
	return -1;
}

void ZMQ_Load_Balancer::Coordinate()
{
	Cout() << "In the coordinate function\n";
}

void ZMQ_Load_Balancer::DoWork(){
	String clientAddr = "";
	if (this->isReadyForWork()){
		LOG("Load balancer: This one is ready for work");
		while(true){
			zmq::poll(items);
			
			//Get messages from port queue
			if (items[0].revents & ZMQ_POLLIN){
				clientAddr = S_recv(&frontend);
				S_recv(&frontend);		//receive empty string delimiter
				String clientMsg = S_recv(&frontend);
				if (clientMsg.Find("HELLO", 0) != -1){
					clientQueue.push(clientAddr);
				}else{
					this->Coordinate();
				}
			}
			if (items[1].revents & ZMQ_POLLIN){
				String workerAddr = S_recv(&backend);
				S_recv(&backend);		//receive empty string delimiter
				String workerMsg = S_recv(&backend);
				if (workerMsg.Find("READY", 0) != -1){
					Cout() << "Have \"READY\" message from worker\n";
					workerQueue.push(workerAddr);
				}else{
					this->Coordinate();
				}
			}
			
			//Process messages
			if (!clientQueue.empty() && !workerQueue.empty()){
				
			}
		}
	}
}

			//Talking with backend
/*
			if (!workerQueue.empty()) {
				//Pop worker from queue
				String workAddr = workerQueue.front();
				workerQueue.pop();
				
				Cout() << "Going to send reply to worker: " << workAddr << "\n";
				Cout() << "Message struct:\n" << workAddr << "\n\n" << "\"message\"" << "\n\n";
				S_send(&backend, workAddr, ZMQ_SNDMORE);
				S_send(&backend, "", ZMQ_SNDMORE);
				S_send(&backend, "Reply message from server");
				//Send client request to free worker
			}
*/
			//Talking with frontend
/*
			if (!clientQueue.empty()){
				//Pop client from queue
				String clAddr = clientQueue.front();
				clientQueue.pop();
				S_send(&frontend, clAddr, ZMQ_SNDMORE);
				S_send(&frontend, "", ZMQ_SNDMORE);
				S_send(&frontend, AsString("Some reply message for " + clAddr));
			}
*/
