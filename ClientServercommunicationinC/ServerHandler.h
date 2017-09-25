#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27020"
class ServerHandler
{
private:
	char recvbuf[DEFAULT_BUFLEN];
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	
	struct sockaddr_in Remaddr;
	struct sockaddr_in server;

	struct addrinfo *result = NULL;
	struct addrinfo hints;
public:
	ServerHandler();
	int MakeAddress(const char* hostname, sockaddr_storage* pAddr);
	int Initialize(char* port);
	int UdpInitialize(char* remoteip,int port);

	SOCKET WaitForConnect();

	void write(char *data);
	void write(int *data,int size);
	void UdpWrite(char* data);
	void UdpWrite(int* data,int size);

	char* read();
	int* readintlist();
	char* Udpread();
	int* Udpreadintlist();

	~ServerHandler();
};

