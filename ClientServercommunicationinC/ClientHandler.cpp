#include "ClientHandler.h"
#include "Sender.h"
#include "Receiver.h"


ClientHandler::ClientHandler()
{
}

int  ClientHandler::Initialize(char* ip,char* portnum) {
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, portnum, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
}
int ClientHandler::UdpInitialize(char* remoteip,int port) {


	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//Create a socket
	if ((ConnectSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	inet_pton(AF_INET, remoteip, &(server.sin_addr));
	server.sin_port = htons(27010);

	//no binding for client

	Remaddr.sin_family = AF_INET;
	Remaddr.sin_port = htons(port);
	inet_pton(AF_INET, remoteip, &(Remaddr.sin_addr));



}
int ClientHandler::Connect() {
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		int iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	return 404;
}

void ClientHandler::write(char* data) {
	Sender senderfromClient(ConnectSocket);
	senderfromClient.Send(data);
}
void ClientHandler::write(int* data,int size) {
	Sender senderfromClient(ConnectSocket);
	senderfromClient.Send(data,size);
}
void ClientHandler::UdpWrite(char* data) {
	Sender senderfromClient(ConnectSocket, Remaddr);
	senderfromClient.Sendto(data);
}
void ClientHandler::UdpWrite(int* data,int size) {
	Sender senderfromClient(ConnectSocket, Remaddr);
	senderfromClient.Sendto(data,size);
}
char* ClientHandler::read() {
	Receiver receivefromclient(ConnectSocket);
	return receivefromclient.Receive();
}
int* ClientHandler::readintlist() {
	Receiver receivefromclient(ConnectSocket);
	return receivefromclient.Receiveintlist();
}
char* ClientHandler::Udpread() {
	Receiver receivefromclient(ConnectSocket, Remaddr);
	return receivefromclient.Receivefrom();
}
int* ClientHandler::Udpreadintlist() {
	Receiver receivefromclient(ConnectSocket, Remaddr);
	return receivefromclient.Receivefromintlist();
}
ClientHandler::~ClientHandler()
{
	if (ConnectSocket)closesocket(ConnectSocket);
}
