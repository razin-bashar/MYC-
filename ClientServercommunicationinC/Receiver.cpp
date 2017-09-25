#include "Receiver.h"
#pragma warning(disable:4996)



Receiver::Receiver(SOCKET soket)
{
	memcpy(&Soket, &soket, sizeof(soket));
	
}

Receiver::Receiver(SOCKET soket, sockaddr_in remaddr) {

	memcpy(&Soket, &soket, sizeof(soket));
	memcpy(&Remaddr, &remaddr, sizeof(remaddr));

}
char* Receiver::Receive() {
	// Receive until the peer closes the connection
	int iResult;
	char fresult[4000];
	int pos = 0;

		iResult = recv(Soket, Buffer, BUFFER_SIZE, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			if (iResult != 512)Buffer[iResult] = NULL;
			printf(Buffer);
			Concat(fresult, Buffer, iResult, pos);
			pos += iResult;
			printf("\n");
			if (Buffer[0] == 'e'&&Buffer[1] == 'n'&&Buffer[2] == 'd') {
				pos = 0;
			}
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());


	return fresult;
}

int* Receiver::Receiveintlist() {
	// Receive until the peer closes the connection
	int iResult;
	char fresult[4000];
	int pos = 0;

	iResult = recv(Soket, Buffer, BUFFER_SIZE, 0);
	if (iResult > 0) {
		printf("Bytes received: %d\n", iResult);
		if (iResult != 512)Buffer[iResult] = NULL;
		printf(Buffer);
		Concat(fresult, Buffer, iResult, pos);
		pos += iResult;
		printf("\n");
		if (Buffer[0] == 'e'&&Buffer[1] == 'n'&&Buffer[2] == 'd') {
			pos = 0;
		}
	}
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());


		return (int*)fresult;
}

char* Receiver::Receivefrom() {
	//printf("hey there");
	char fresult[4000];
	int pos = 0;
	int addlen = sizeof(Remaddr);
	int iResult;
	do {
		iResult = recvfrom(Soket, Buffer, BUFFER_SIZE, 0, (sockaddr*)&Remaddr, &addlen);
		if (iResult > 0) {
			//printf("Bytes received: %d\n", iResult);
			if (iResult != 512)Buffer[iResult] = NULL;
			printf(Buffer);
			Concat(fresult, Buffer, iResult, pos);
			pos += iResult;
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());
	} while (iResult > 0);
	return fresult;
}

int* Receiver::Receivefromintlist() {
	//printf("hey there");
	char fresult[4000];
	int pos = 0;
//	strcpy(fresult, "");
	int addlen = sizeof(Remaddr);
	int iResult;
		do{
			iResult = recvfrom(Soket, Buffer, BUFFER_SIZE, 0, (sockaddr*)&Remaddr, &addlen);
			if (iResult > 0) {
				//printf("Bytes received: %d\n", iResult);
				if (iResult != 512)Buffer[iResult] = NULL;
				//printf(Buffer);
				GetIntList(Buffer, iResult);
				Concat(fresult, Buffer, iResult, pos);
				pos += iResult;
				printf("\n");
				
			}
			else if (iResult == 0)
				printf("Connection closed\n");
			else
				printf("recv failed with error: %d\n", WSAGetLastError());
		} while (iResult > 0);

		return (int*)fresult;
}
void Receiver::Concat(char destination[],char source[],int size,int position) {
	for (int i = 0; i < size; i++) {
		destination[position + i] = source[i];
	}
	if (size < BUFFER_SIZE)destination[position + size] = NULL;
}
int* Receiver::GetIntList(char* data,int size) {
	int* result;
	int len = size / 4;
	result = (int*)data;
	for (int i = 0; i < len;i++) {
		printf("%d ", result[i]);
	}
	return result;
}
Receiver::~Receiver()
{
	// cleanup
	//closesocket(Soket);
}
