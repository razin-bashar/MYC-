#include "IndependentClient.h"
#include "IndependentServer.h"
#include "AudioChatRoomController.h"
#include "IndependentTcpServer.h"
#include<cstdio>
#include<string>
#include <windows.h>
#include <iostream>
#include <thread>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdlib.h>
#include "Node.h"
#pragma warning(disable:4996)

#define INADDR_NONE 0xffffffff
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")


uint32_t parseIPV4string(char const * ipAddress)
{
	unsigned int ip[4];

	if (4 != sscanf(ipAddress, "%u.%u.%u.%u", &ip[0], &ip[1], &ip[2], &ip[3]))
		return 0;   // or some other indicator or error

	return ip[3] + ip[2] * 0x100 + ip[1] * 0x10000ul + ip[0] * 0x1000000ul;
}
int PacketRoundTrip(char* ipadress)
{

	HANDLE hIcmpFile;
	unsigned long ipaddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	char SendData[32] = "Data Buffer";
	LPVOID ReplyBuffer = NULL;
	DWORD ReplySize = 0;
	PICMP_ECHO_REPLY pEchoReply;
	//ipaddr = parseIPV4string(ipadress);
	ipaddr = inet_addr(ipadress);

	if (ipaddr == INADDR_NONE) {
		//	printf("usage: %s IP address\n", argv[0]);
		return 1;
	}

	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) {
		printf("\tUnable to open handle.\n");
		printf("IcmpCreatefile returned error: %ld\n", GetLastError());
		return 1;
	}

	ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
	ReplyBuffer = (VOID*)malloc(ReplySize);
	if (ReplyBuffer == NULL) {
		printf("\tUnable to allocate memory\n");
		return 1;
	}


	dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
		NULL, ReplyBuffer, ReplySize, 1000);
	if (dwRetVal != 0) {
		pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
		struct in_addr ReplyAddr;
		ReplyAddr.S_un.S_addr = pEchoReply->Address;
		//printf("\tSent icmp message to %s\n", ipadress);
		//if (dwRetVal > 1) {
		//	printf("\tReceived %ld icmp message responses\n", dwRetVal);
		//	printf("\tInformation from the first response:\n");
		//}
		//else {
		//	printf("\tReceived %ld icmp message response\n", dwRetVal);
		//	printf("\tInformation from this response:\n");
		//}
		//printf("\t  Received from %s\n", inet_ntoa(ReplyAddr));
		//printf("\t  Status = %ld\n",
		//	pEchoReply->Status);
		//printf("\t  Roundtrip time = %ld milliseconds\n",
		//	pEchoReply->RoundTripTime);
	}
	else {
		//printf("\tCall to IcmpSendEcho failed.\n");
		//printf("\tIcmpSendEcho returned error: %ld\n", GetLastError());
		return 1000;
	}
	return pEchoReply->RoundTripTime;
}
int main() {

	
	
	//std::thread server(&Pc1::UdpRun,Pc1());
	//std::thread client(&Pc2::UdpRun,Pc2());

	//server.join();
	//client.join();

	//	int i = PacketRoundTrip("192.168.29.14");
	//printf(" %d ", i);
	
	//AudioChatRoomController ac("192.168.29.14");
	//ac.add("192.168.29.15");

	std::thread server(&IndependentTcpServer::Run, IndependentTcpServer(Node::getInstance()->port));

	AudioChatRoomController AR;
	AR.add("192.168.28.198", "27020");
	//Node* N = Node::getInstance();
	Sleep(2000);
	//AudioChatRoomController AR;
	//AR.add("192.168.28.205", "27020");

	server.join();
	getchar();

	//jamela ache onk boro
	/*int a = 1200;
	int b = (unsigned char)(a);
	printf("%d", b);
	getchar();*/
	return 0;
}