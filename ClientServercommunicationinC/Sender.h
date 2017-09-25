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

#define BUFFER_SIZE 512
class Sender
{
private:
	SOCKET Soket;
	sockaddr_in Remaddr;
public:
	Sender(SOCKET soket);
	Sender(SOCKET soket, struct sockaddr_in remaddr);
	int Send(char* sender);
	int Send(int* sender,int size);
	int Sendto(char* sender);
	int Sendto(int* sender,int size);
	~Sender();
};

