#pragma once
#include "Address.h"
#include <winsock.h>
#include <string>
#pragma warning(disable:4996)
#define CLUSTER_NUM 2
#define CLUSTER_SIZE 2

class Node
{
private:
	static bool instanceFlag;
	static Node *single;
	Node()
	{
		ip=getMyIp();
		port = "27020";
		Head = new Address[ClusterNumber];
		Worker_under_head = new int[ClusterNumber];
		worker = new Address[ClusterSize];

		for (int i = 0; i < CLUSTER_NUM; i++) {
			Node::Head[i].ip = "";
			Node::Head[i].port = "";
			Node::Worker_under_head[i] = 0;
		}
		for (int i = 0; i < CLUSTER_SIZE; i++) {
			worker[i].ip = "";
			worker[i].port = "";
		}
		headnode = 0;
		workernode = 0;
		IsHead = false;
		update1 = false;
		update2 = false;
		whichnode = -1;

	}

public:
	const int ClusterNumber = 2;
	const int ClusterSize = 2;

	char* ip; 
	char* port;

	Address* Head;
	int* Worker_under_head;
	bool IsHead;
	Address* worker;
	int workernode;
	int headnode;
	int whichnode;
	bool update1;
	bool update2;
	Address Head_of_worker;
	static Node* getInstance();
	char* toString();
	void print();


	char* getMyIp() {
		WSADATA wsa_Data;
		int wsa_ReturnCode = WSAStartup(0x101, &wsa_Data);

		// Get the local hostname
		char szHostName[255];
		gethostname(szHostName, 255);
		struct hostent *host_entry;
		host_entry = gethostbyname(szHostName);
		char * szLocalIP;
		szLocalIP = inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);
		WSACleanup();
		return szLocalIP;
	}
	
	~Node()
	{
		instanceFlag = false;
	}
};
