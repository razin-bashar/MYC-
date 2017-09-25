#pragma once
#include <WinSock2.h>
#define CLUSTER_NUM 4
#define CLUSTER_SIZE 10
class SingleServer
{
public:
	SOCKET socket;
	int Id;
	char* msg;
public:
	SingleServer();
	SingleServer(SOCKET sc,int id);
	void Run();
	void SendtoallHead();
	char* ProcessCluster(char* clusterinfo);
	void Updatenode(char* clusterinfo);
	int ICMPtrip(char* ipadress);
	~SingleServer();
};

