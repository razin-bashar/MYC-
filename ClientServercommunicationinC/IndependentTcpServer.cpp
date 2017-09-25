#include "IndependentClient.h"
#include "SingleServer.h"
#include "AudioChatRoomController.h"
#include "HeadSenderThread.h"
#include "IndependentTcpServer.h"
#include "Sender.h"
#include "Receiver.h"
#include "ServerHandler.h"
#include "Address.h"
#include "Node.h"
#include "Collections.h"

#include<cstdio>
#include<string.h>
#include<string>
#include <sstream>
#include <algorithm>
#include <iterator> 
#include <vector>
#include <windows.h>
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdlib.h>
#include <Ws2tcpip.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable:4996)

#define INADDR_NONE 0xffffffff
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")


IndependentTcpServer::IndependentTcpServer(char* port)
{
	myport = port;
}
void IndependentTcpServer::Updatenode(char* clusterinfo) {

	int length, Headnode, Workernode;
	Address Head[CLUSTER_SIZE];
	int Worker_under_head[CLUSTER_NUM];

	std::string cluster(clusterinfo);
	std::istringstream iss(cluster);
	std::vector<std::string> splited{ std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{} };

	length = splited.size();
	Node* node = Node::getInstance();
	printf(" %s ", node->toString());
	int hit = node->headnode;
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
	printf(" %s ", node->toString());
}
void IndependentTcpServer::Run() {
	printf("Server");

	ServerHandler servermaker;

	servermaker.Initialize(myport);
	
	int id = 0;
	while (true) {
		SOCKET sc = servermaker.WaitForConnect();
		std::thread* x= new std::thread(&SingleServer::Run, SingleServer(sc,++id));
		x->join();

	}
}



void IndependentTcpServer::SendtoallHead() {
	Node* node = Node::getInstance();
	int num = node->headnode;
	HeadSenderThread *sen[CLUSTER_NUM];
	std::thread *senn[CLUSTER_NUM];
	//std::thread server(&IndependentTcpServer::Run, IndependentTcpServer());
	if (node->IsHead) {
		std::string headinfor("");
		headinfor.append("HEAD ");
		headinfor.append(node->ip);
		headinfor.append(" ");
		headinfor.append(node->port);
		headinfor.append(" ");
		headinfor.append(std::to_string(node->workernode));


		char *notificationmsg = (char *)alloca(headinfor.size() + 1);
		memcpy(notificationmsg, headinfor.c_str(), headinfor.size() + 1);

		for (int i = 0; i < num - 1; i++) {

			char * ip = Collections::strtochar(node->Head[i].ip);

			char * port = Collections::strtochar(node->Head[i].port); 

			sen[i] = new HeadSenderThread(ip, port, notificationmsg);
			senn[i] = new std::thread(&HeadSenderThread::Run, sen[i]);
		}

	}
}
char* IndependentTcpServer::ProcessCluster(char* clusterinfo) {

	int length, Headnode, Workernode;
	Address Head[CLUSTER_NUM];
	int Worker_under_head[CLUSTER_NUM];

	std::string cluster(clusterinfo);
	std::istringstream iss(cluster);
	std::vector<std::string> splited{ std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{} };

	length = splited.size();

	int hit = 0;
	for (int i = 0; i < length; i++) {
		std::string what = splited.at(i);
		if (what == "HN")Headnode = std::stoi(splited.at(++i));
		else if (what == "Head") {
			Head[hit].ip = splited.at(++i);
			Head[hit].port = splited.at(++i);
			Worker_under_head[hit++] = std::stoi(splited.at(++i));
		}
	}

	printf("%d", length);

	if (Headnode < CLUSTER_NUM) { //era hobe head. head hole headlist rakbe

		Node* node = Node::getInstance();
		node->IsHead = true;
		for (int i = 0; i < Headnode; i++) {
			node->Head[i].ip = std::string(Head[i].ip);
			node->Head[i].port = std::string(Head[i].port);
			node->Worker_under_head[i] = Worker_under_head[i];
		}

		node->Head[Headnode].ip = node->ip;
		node->Head[Headnode].port = node->port;
		node->Worker_under_head[Headnode] = 0;
		node->headnode = Headnode + 1;
		node->workernode = 0;

		return node->toString();
	}
	else {	//ere hobe worker. worker hole shudhu ter head ke rakbe

	}


	return (char*)NULL;

}
int IndependentTcpServer::ICMPtrip(char* ipadress) {
	HANDLE hIcmpFile;
	unsigned long ipaddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	char SendData[32] = "Data Buffer";
	LPVOID ReplyBuffer = NULL;
	DWORD ReplySize = 0;
	PICMP_ECHO_REPLY pEchoReply;

	ipaddr = inet_addr(ipadress);
	if (ipaddr == INADDR_NONE) {
		//	printf("usage: %s IP address\n", argv[0]);
		return 1;
	}

	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) {
		printf("\tUnable to open handle.\n");
		printf("IcmpCreatefile returned error: %ld\n", GetLastError());
		return 1;
	}

	ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
	ReplyBuffer = (VOID*)malloc(ReplySize);
	if (ReplyBuffer == NULL) {
		printf("\tUnable to allocate memory\n");
		return 1;
	}


	dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
		NULL, ReplyBuffer, ReplySize, 1000);
	if (dwRetVal != 0) {
		pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
		struct in_addr ReplyAddr;
		ReplyAddr.S_un.S_addr = pEchoReply->Address;
		//printf("\tSent icmp message to %s\n", ipadress);
		//if (dwRetVal > 1) {
		//	printf("\tReceived %ld icmp message responses\n", dwRetVal);
		//	printf("\tInformation from the first response:\n");
		//}
		//else {
		//	printf("\tReceived %ld icmp message response\n", dwRetVal);
		//	printf("\tInformation from this response:\n");
		//}
		//printf("\t  Received from %s\n", inet_ntoa(ReplyAddr));
		//printf("\t  Status = %ld\n",
		//	pEchoReply->Status);
		//printf("\t  Roundtrip time = %ld milliseconds\n",
		//	pEchoReply->RoundTripTime);
	}
	else {
		//printf("\tCall to IcmpSendEcho failed.\n");
		//printf("\tIcmpSendEcho returned error: %ld\n", GetLastError());
		return 1000;
	}
	return pEchoReply->RoundTripTime;
}
IndependentTcpServer::~IndependentTcpServer()
{
}
