#pragma once
#define CLUSTER_NUM 4
#define CLUSTER_SIZE 10
class IndependentServer
{

public:

	char* myport;
public:
	IndependentServer(char* port);
	void Run();
	void Updatenode(char* clusterinfo);
	void UdpRun();
	~IndependentServer();
};

