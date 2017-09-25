#pragma once
class HeadSenderThread
{
public:
	char* Ip;
	char* Port;
	char* Data;
public:
	HeadSenderThread(char* ip, char* port, char* data);
	void Run();
	~HeadSenderThread();
};

