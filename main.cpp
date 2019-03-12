#include "main.h"

CONSOLE_APP_MAIN
{
	LOG("***Load balancer started doing it's work***");
	ZMQ_Load_Balancer lb;
	
	lb.Bind("tcp://*:5555", "tcp://*:5556");
		
	Cout() << "Load balancer: Working\n";
	lb.DoWork();
	Cout() << "Load balancer: Finished working\n";
	
	LOG("***Load balancer finished it's work***");
}