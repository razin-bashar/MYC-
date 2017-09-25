#pragma once

#define CLUSTER_NUM 4
#define CLUSTER_SIZE 10
class IndependentTcpServer
{
public:
	char* myport;
public:
	IndependentTcpServer(char *port);
	void Run();
	void SendtoallHead();
	char* ProcessCluster(char* clusterinfo);
	void Updatenode(char* clusterinfo);
	
	int ICMPtrip(char* ipadress);
	~IndependentTcpServer();
};

