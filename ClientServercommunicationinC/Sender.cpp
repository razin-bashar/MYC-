#include "Sender.h"


Sender::Sender(SOCKET soket)
{
	memcpy(&Soket,&soket,sizeof(soket));
}
Sender::Sender(SOCKET soket,sockaddr_in remaddr) {

	memcpy(&Soket, &soket, sizeof(soket));
	memcpy(&Remaddr, &remaddr, sizeof(remaddr));

}
int Sender::Send( char *data) {
	int totalsize = (int)strlen(data)*sizeof(char);
	int remaining = totalsize;
	int iResult;
	while (true) {
		iResult = sendto(Soket, data, (remaining>BUFFER_SIZE) ? BUFFER_SIZE : remaining, 0, (sockaddr*)&Remaddr, sizeof(Remaddr));
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(Soket);
			WSACleanup();
			return 1;
		}
		data = &data[BUFFER_SIZE];
		remaining -= BUFFER_SIZE;
		printf("Bytes Sent: %ld\n", iResult);
		if (remaining < 0)break;

	}
}
int Sender::Send(int *data,int size) {
	char* intdata = (char*)data;
	int totalsize = 4 * size*sizeof(char);
	int remaining = totalsize;
	int iResult;
	while (true) {
		iResult = sendto(Soket, intdata, (remaining>BUFFER_SIZE) ? BUFFER_SIZE : remaining, 0, (sockaddr*)&Remaddr, sizeof(Remaddr));
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(Soket);
			WSACleanup();
			return 1;
		}
		intdata = &intdata[BUFFER_SIZE];
		remaining -= BUFFER_SIZE;
		printf("Bytes Sent: %ld\n", iResult);
		if (remaining < 0)break;

	}
}
int Sender::Sendto(char *data) {
	int totalsize =(int)strlen(data)*sizeof(char);
	int remaining = totalsize;
	int iResult;
	while (true) {
		iResult = sendto(Soket, data, (remaining>BUFFER_SIZE) ? BUFFER_SIZE : remaining, 0, (sockaddr*)&Remaddr, sizeof(Remaddr));
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(Soket);
			WSACleanup();
			return 1;
		}
		data = &data[BUFFER_SIZE];
		remaining -= BUFFER_SIZE;
		printf("Bytes Sent: %ld\n", iResult);
		if (remaining < 0)break;

	}
}
int Sender::Sendto(int *data,int size) {
	char* intdata = (char*)data;

	int totalsize = 4 * size*sizeof(char);
	int remaining = totalsize;
	int iResult;
	while(true) {
		iResult = sendto(Soket, intdata, (remaining>BUFFER_SIZE)?BUFFER_SIZE:remaining, 0, (sockaddr*)&Remaddr, sizeof(Remaddr));
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(Soket);
			WSACleanup();
			return 1;
		}
		intdata = &intdata[BUFFER_SIZE];
		remaining -= BUFFER_SIZE;
		printf("Bytes Sent: %ld\n", iResult);
		if (remaining < 0)break;
		
	}

}
Sender::~Sender()
{
	/*int iResult = shutdown(Soket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(Soket);
		WSACleanup();
		//return 1;
	}*/

}
