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

//#define DEFAULT_PORT "27020"
class ClientHandler
{
private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,*ptr = NULL,hints;
	struct sockaddr_in Remaddr;
	struct sockaddr_in server;

	char *sendbuf = "this is a test dnfjlkdf jdfglkfjdlk jfdlgjldkfg ijfojgkdfjl kljfgfk fjgkldf\
						is a test dnfjlkdf jdfglkfjdlk jfdlgjldkfg ijfojgkdfjl kljfgfk fjgkldf\
						this is a test dnfjlkdf jdfglkfjdlk jfdlgjldkfg ijfojgkdfjl kljfgfk fjgkldf\
						this is a test dnfjlkdf jdfglkfjdlk jfdlgjldkfg ijfojgkdfjl kljfgfk fjgkldf\
						this is a test dnfjlkdf jdfglkfjdlk jfdlgjldkfg ijfojgkdfjl kljfgfk fjgkldf\
						this is a test dnfjlkdf jdfglkfjdlk jfdlgjldkfg ijfojgkdfjl kljfgfk fjgkldf\
						this is a test dnfjlkdf jdfglkfjdlk jfdlgjldkfg ijfojgkdfjl kljfgfk fjgkldf\
						this is a test dnfjlkdf jdfglkfjdlk jfdlgjldkfg ijfojgkdfjl kljfgfk fjgkldf";
public:
	ClientHandler();

	int Initialize(char *ip, char* portnum);
	int UdpInitialize(char* remoteip,int port);

	int Connect();

	void write(char *data);
	void write(int *data,int size);
	void UdpWrite(char* data);
	void UdpWrite(int* data,int size);

	char* read();
	int* readintlist();
	char* Udpread();
	int* Udpreadintlist();

	~ClientHandler();
};

