#include "IndependentServer.h"
#include "ServerHandler.h"
#include "Sender.h"
#include "Receiver.h"
#include "Address.h"
#include "Node.h"
#include<cstdio>
#include<string>
#include <sstream>
#include <vector>
#include <iterator>
#include <windows.h>
#include <winsock2.h>
#include <winsock.h>


IndependentServer::IndependentServer(char* port)
{
	myport = port;
}

void IndependentServer::Updatenode(char* clusterinfo) {
	int length, Headnode, Workernode;
	Address Head[CLUSTER_NUM];
	int Worker_under_head[CLUSTER_NUM];

	std::string cluster(clusterinfo);
	std::istringstream iss(cluster);
	std::vector<std::string> splited{ std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{} };

	length = splited.size();
	Node* node = Node::getInstance();
	printf(" %s ", node->toString());
	int hit =node->headnode ;
	for (int i = 0; i < length; i++) {
		std::string what = splited.at(i);
		if (what == "HN")Headnode = std::stoi(splited.at(++i));
		else if (what == "HEAD") {
			node->Head[hit].ip = splited.at(++i);
			node->Head[hit].port = splited.at(++i);
			node->Worker_under_head[hit] = std::stoi(splited.at(++i));
			node->headnode++;
		}
	}
	printf(" %s ",node->toString());
}
void IndependentServer::Run() {
	/*printf("Server");

	ServerHandler servermaker;

	servermaker.Initialize(myport);
	while (true) {
		servermaker.WaitForConnect();
		char* res=servermaker.read();
		if (strcmp(res, "UPDATED_HEAD") == 0)
		{
			res = servermaker.read();
			Updatenode(res);
			
		}
	}*/

	/*Sender sender(Channel);
	Receiver receiver(Channel);
	for (int i = 0; i < 500; i++) {
		printf("\n %d \n",i);
	}
	char* msg = receiver.Receive();
	printf("out %s", msg);
	if (strcmp(msg, "ADD_REQ") == 0) {
		sender.Send("ADD_YES");
		char* headlist = receiver.Receive();
		printf("=======%s", headlist);
		//char* modifiedheadlist = ProcessCluster(headlist);
		sender.Send("DONE");
		//printf(" |||| %s ||| ", modifiedheadlist);
		//SendtoallHead();
	}
	else if (strcmp(msg, "UPDATED_HEAD") == 0)
	{
		msg = receiver.Receive();
		//Updatenode(msg);

	}*/
}
void IndependentServer::UdpRun() {
	printf("Server");

	ServerHandler servermaker;

	servermaker.UdpInitialize("192.168.29.121",27010);
	while (true) {
		servermaker.Udpreadintlist();
	}

}
IndependentServer::~IndependentServer()
{
}
