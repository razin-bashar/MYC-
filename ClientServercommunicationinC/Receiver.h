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
class Receiver
{
private:
	SOCKET Soket;
	char Buffer[BUFFER_SIZE];
	sockaddr_in Remaddr;

public:
	Receiver(SOCKET soket);
	Receiver(SOCKET soket, sockaddr_in remaddr);
	char* Receive();
	char* Receivefrom();
	int* Receiveintlist();
	int* Receivefromintlist();
	void Concat(char destination[], char source[], int size, int position);
	int* GetIntList(char* data,int size);
	~Receiver();
};

