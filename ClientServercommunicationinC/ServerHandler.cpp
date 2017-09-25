#include "ServerHandler.h"
#include "Sender.h"
#include "Receiver.h"


ServerHandler::ServerHandler()
{
}
int ServerHandler::MakeAddress(const char* hostname, sockaddr_storage* pAddr)
{
	int result;
	addrinfo* result_list = NULL;
	addrinfo hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM; // without this flag, getaddrinfo will return 3x the number of addresses (one for each socket type).
	result = getaddrinfo(hostname, DEFAULT_PORT, &hints, &result_list);
	if (result == 0)
	{
		//ASSERT(result_list->ai_addrlen <= sizeof(sockaddr_in));
		memcpy(pAddr, result_list->ai_addr, result_list->ai_addrlen);
		freeaddrinfo(result_list);
	}

	return result;
}

int ServerHandler::Initialize(char* port) {
	
	int iResult;



	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

}

int ServerHandler::UdpInitialize(char* remoteip,int port) {
	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//Create a socket
	if ((ClientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	inet_pton(AF_INET, remoteip, &(server.sin_addr));
	server.sin_port = htons(port);

	//Bind
	if (bind(ClientSocket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");

	Remaddr.sin_family = AF_INET;
	Remaddr.sin_port = htons(port);
	inet_pton(AF_INET, remoteip, &(Remaddr.sin_addr));
}
SOCKET ServerHandler::WaitForConnect() {

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return ClientSocket;
	}
	return ClientSocket;
	// No longer need server socket
	//closesocket(ListenSocket);
}

void ServerHandler::write(char* data) {
	Sender senderfromClient(ClientSocket);
	senderfromClient.Send(data);
}
void ServerHandler::write(int* data,int size) {
	Sender senderfromClient(ClientSocket);
	senderfromClient.Send(data,size);
}
void ServerHandler::UdpWrite(char* data) {
	Sender senderfromClient(ClientSocket, Remaddr);
	senderfromClient.Sendto(data);
}
void ServerHandler::UdpWrite(int* data,int size) {
	Sender senderfromClient(ClientSocket, Remaddr);
	senderfromClient.Sendto(data,size);
}
char* ServerHandler::read() {
	Receiver receivefromclient(ClientSocket);
	return receivefromclient.Receive();
}
int* ServerHandler::readintlist() {
	Receiver receivefromclient(ClientSocket);
	return receivefromclient.Receiveintlist();
}
char* ServerHandler::Udpread() {
	Receiver receivefromclient(ClientSocket, Remaddr);
	return  receivefromclient.Receivefrom();
	
}
int* ServerHandler::Udpreadintlist() {
	Receiver receivefromclient(ClientSocket, Remaddr);
	return receivefromclient.Receivefromintlist();
}
ServerHandler::~ServerHandler()
{
}
