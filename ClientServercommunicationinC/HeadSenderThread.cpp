#include "HeadSenderThread.h"
#include "ClientHandler.h"


HeadSenderThread::HeadSenderThread(char* ip, char* port, char* data)
{
	Ip = ip;
	Port = port;
	Data = data;

}

void HeadSenderThread::Run() {


	printf(" %s %s %s ", Ip, Port, Data);
	ClientHandler sendertohead;
	sendertohead.Initialize(Ip, Port);
	sendertohead.Connect();
	sendertohead.write("UPDATED_HEAD");
	char* msg = sendertohead.read();
	if (strcmp(msg, "yes") == 0)sendertohead.write(Data);

}

HeadSenderThread::~HeadSenderThread()
{
}
