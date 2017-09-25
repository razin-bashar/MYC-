#include "SingleServer.h"
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
#include <mutex>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable:4996)

#define INADDR_NONE 0xffffffff
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")


std::mutex code;

SingleServer::SingleServer()
{
}

SingleServer::SingleServer(SOCKET sc, int id) {
	socket = sc;
	Id = id;
}
void SingleServer::Run() {
	Sender sender(socket);
	Receiver receiver(socket);
	/*for (int i = 0; i < 500; i++) {
	printf("\n %d \n", i);
	}*/

	printf("Client Id=%d ", Id);
	msg = receiver.Receive();
	printf("out %s", msg);
	if (strcmp(msg, "ADD_REQ") == 0) {
		sender.Send("ADD_YES");
		char* headlist = receiver.Receive();
		printf("=======%s", headlist);
		char* modifiedheadlist = ProcessCluster(headlist);
		sender.Send("DONE");
		if (strcmp(modifiedheadlist, "preexist") == 0)return;
		printf(" |||| %s ||| ", modifiedheadlist);
		SendtoallHead();
	}
	else if (strcmp(msg, "UPDATED_HEAD") == 0)
	{
		sender.Send("yes");
		msg = receiver.Receive();
		Updatenode(msg);
		Node::getInstance()->print();

	}
}
void SingleServer::Updatenode(char* clusterinfo) {
	int length, Headnode, Workernode;
	Address Head[CLUSTER_NUM];
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
		std::cout << "("<<what<<")";
		if (what == "HN")Headnode = std::stoi(splited.at(++i));
		else if (what == "HEAD") {
			node->Head[hit].ip = splited.at(++i);
			node->Head[hit].port = splited.at(++i);
			node->Worker_under_head[hit] = std::stoi(splited.at(++i));
			node->headnode++;
		}
		else if (what == "WORKER") {
			std::string workerip = splited.at(1);
			std::string workerport = splited.at(2);

			std::string ownerip = splited.at(3);
			std::string ownerport = splited.at(4);

			std::string  index = splited.at(5);
			if (index == "Index") {
				std::string pos = splited.at(6);
				int position = std::stoi(pos);
				if (position >= 0 && position < node->headnode) {
					std::unique_lock<std::mutex>lck(code);
					node->Worker_under_head[position]++;
					lck.unlock();
					if (node->ip == ownerip && node->port == ownerport) {
						int w_num = node->workernode;
						std::unique_lock<std::mutex>lck(code);

						node->worker[w_num].ip = workerip;
						node->worker[w_num].port = workerport;
						node->workernode = w_num + 1;
						lck.unlock();
					}
				}
			}
		}
	}
	printf(" %s ", node->toString());
}
void SingleServer::SendtoallHead() {

	Node* node = Node::getInstance();

	if (node->IsHead) {

		int num = node->headnode;
		HeadSenderThread *sen[CLUSTER_NUM];
		std::thread *senn[CLUSTER_NUM];

		//std::thread server(&IndependentTcpServer::Run, IndependentTcpServer());

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

			char * ip = (char *)alloca(node->Head[i].ip.size() + 1);
			memcpy(ip, node->Head[i].ip.c_str(), node->Head[i].ip.size() + 1);

			char * port = (char *)alloca(node->Head[i].port.size() + 1);
			memcpy(port, node->Head[i].port.c_str(), node->Head[i].port.size() + 1);

			sen[i] = new HeadSenderThread(ip, port, notificationmsg);
			senn[i] = new std::thread(&HeadSenderThread::Run, sen[i]);
		}
		for (int i = 0; i < num - 1; i++) {
			senn[i]->join();
		}
	}
	else {

		int num = node->headnode;
		HeadSenderThread *sen[CLUSTER_NUM];
		std::thread *senn[CLUSTER_NUM];

		//std::thread server(&IndependentTcpServer::Run, IndependentTcpServer());

		std::string headinfor("");
		headinfor.append("WORKER ");
		headinfor.append(node->ip);
		headinfor.append(" ");
		headinfor.append(node->port);
		headinfor.append(" ");
		headinfor.append(node->Head_of_worker.ip);
		headinfor.append(" ");
		headinfor.append(node->Head_of_worker.port);
		headinfor.append(" ");
		headinfor.append("Index");
		headinfor.append(" ");
		headinfor.append(std::to_string(node->whichnode));

		char *notificationmsg = (char *)alloca(headinfor.size() + 1);
		memcpy(notificationmsg, headinfor.c_str(), headinfor.size() + 1);

		for (int i = 0; i < num; i++) {

			char * ip = (char *)alloca(node->Head[i].ip.size() + 1);
			memcpy(ip, node->Head[i].ip.c_str(), node->Head[i].ip.size() + 1);

			char * port = (char *)alloca(node->Head[i].port.size() + 1);
			memcpy(port, node->Head[i].port.c_str(), node->Head[i].port.size() + 1);

			sen[i] = new HeadSenderThread(ip, port, notificationmsg);
			senn[i] = new std::thread(&HeadSenderThread::Run, sen[i]);
		}
		for (int i = 0; i < num - 1; i++) {
			senn[i]->join();
		}
	}
}
char* SingleServer::ProcessCluster(char* clusterinfo) {

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

	Node* node = Node::getInstance();
	//lock
	std::unique_lock<std::mutex>lck(code);
	if (node->update1)return "preexist";
	node->IsHead = true;
	for (int i = 0; i < Headnode; i++) {
		node->Head[i].ip = std::string(Head[i].ip);
		node->Head[i].port = std::string(Head[i].port);
		node->Worker_under_head[i] = Worker_under_head[i];
	}
	node->headnode = Headnode;
	node->update1 = true;
	lck.unlock();

	if (Headnode < CLUSTER_NUM) { //era hobe head. head hole headlist rakbe



		node->Head[Headnode].ip = node->ip;
		node->Head[Headnode].port = node->port;
		node->Worker_under_head[Headnode] = 0;
		node->headnode = Headnode + 1;
		node->workernode = 0;

		return node->toString();
	}
	else {	//ere hobe worker. worker hole shudhu ter head ke rakbe
		Node* node = Node::getInstance();
		bool isequall = true;


		int init = node->Worker_under_head[0];
		for (int i = 1; i < node->headnode; i++) {
			if (init != node->Worker_under_head[i]) {
				isequall = false;
				break;
			}
		}


		int index = -1;

		int minsize = node->Worker_under_head[0];
		char * ip = (char *)alloca(node->Head[0].ip.size() + 1);
		memcpy(ip, node->Head[0].ip.c_str(), node->Head[0].ip.size() + 1);

		char * port = (char *)alloca(node->Head[0].port.size() + 1);
		memcpy(port, node->Head[0].port.c_str(), node->Head[0].port.size() + 1);

		if (isequall) {
			int min = 10000;
			for (int i = 0; i < node->headnode; i++) {
				int totaltime = 0;
				char* tip = (char *)alloca(node->Head[i].ip.size() + 1);
				memcpy(tip, node->Head[i].ip.c_str(), node->Head[i].ip.size() + 1);

				for (int j = 0; j < 3; j++) {
					totaltime += ICMPtrip(tip);
				}
				int avgtime = (double)totaltime / 3;
				if (min > avgtime) {
					min = avgtime;
					index = i;
					memcpy(ip, node->Head[i].ip.c_str(), node->Head[i].ip.size() + 1);
					memcpy(port, node->Head[i].port.c_str(), node->Head[i].port.size() + 1);
				}
			}

		}
		else {

			for (int i = 1; i < node->headnode; i++) {
				if (minsize > node->Worker_under_head[i]) {

					minsize = node->Worker_under_head[i];
					index = i;
					ip = (char *)alloca(node->Head[i].ip.size() + 1);
					memcpy(ip, node->Head[i].ip.c_str(), node->Head[i].ip.size() + 1);

					port = (char *)alloca(node->Head[i].port.size() + 1);
					memcpy(port, node->Head[i].port.c_str(), node->Head[i].port.size() + 1);

				}
			}

		}
		std::unique_lock<std::mutex>lck(code);
		if (index >= 0) { node->Worker_under_head[index] += 1; node->whichnode = index; }
		lck.unlock();
		node->IsHead = false;
		node->Head_of_worker.ip.assign(ip);////////////////////////////////////////////////////////////////////////////////////////////////////
		node->Head_of_worker.port.assign(port);////

		return node->toString();
	}


}
int SingleServer::ICMPtrip(char* ipadress) {
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
SingleServer::~SingleServer()
{
}
