#include "AudioChatRoomController.h"
#include "ClientHandler.h"
#include "Node.h"
#include <stdio.h>

AudioChatRoomController::AudioChatRoomController()
{
	//memcpy(&ActiveCluster[0].Head, &ip; (int)strlen(ip)*sizeof(char));
	Node *node = Node::getInstance();
	if (node->headnode == 0) {
		node->Head[node->headnode].ip = node->ip;
		node->Head[node->headnode].port = node->port;
		node->IsHead = true;
		node->headnode++;

	}
}

int AudioChatRoomController::add(char* ip, char* port) {

	Node *node = Node::getInstance();
	if (node->IsHead) {

		ClientHandler client;
		client.Initialize(ip, port);
		if (client.Connect() == 404) {
			client.write("ADD_REQ");
			char* res = client.read();
			printf("an %s", res);
			if (strcmp(res, "ADD_YES") == 0) {
				
				char* headlist =node->toString();
				client.write(headlist);
				char* isclusterok = client.read();
				if (strcmp(isclusterok, "DONE"))return 1;
				else return 0;
			}
		}
	}
	return 0;
}

int AudioChatRoomController::remove(char* ip,char* port) {
	return 1;
}

AudioChatRoomController::~AudioChatRoomController()
{
}
