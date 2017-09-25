#include "IndependentClient.h"
#include "ClientHandler.h"


IndependentClient::IndependentClient()
{
}

void IndependentClient::Run() {
	printf("Client");
	ClientHandler clientmaker;
	clientmaker.Initialize("192.168.29.121","27010");
	clientmaker.Connect();
	clientmaker.write("From Laptop");
	//clientmaker.read();
}

void IndependentClient::UdpRun() {
	printf("Client");
	ClientHandler clientmaker;
	clientmaker.UdpInitialize("192.168.29.121",27010);
	while (true) {

		//clientmaker.UdpWrite("From Laptop");
		//int a[] = { 41,512,1200,2048 };
		int* a;
		a = (int*)malloc(sizeof(int) * 500);
		for (int i = 0; i < 500; i++) {
			a[i] = i * 100;
		}
		while (true) {
			clientmaker.UdpWrite(a, 500);
			clientmaker.UdpWrite("end");
		}
	}
	//clientmaker.read();
}
IndependentClient::~IndependentClient()
{
}
