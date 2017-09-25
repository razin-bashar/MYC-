#include "Node.h"
#include <iostream>





bool Node::instanceFlag = false;
Node* Node::single = NULL;

Node* Node::getInstance()
{
	if (!instanceFlag)
	{
		single = new Node();
		instanceFlag = true;
		return single;
	}
	else
	{
		return single;
	}
}

void Node::print() {
	printf("\n\n  ******************************************** NODE INFORMATION ******************************************  \n\n");
	if (IsHead) printf("\n  HEAD\n");
	else printf("\n  WORKER\n");
	printf("\n  IP: %s:%s\n",ip,port);
	printf("\n  ******************************************** HEAD NODE ******************************************  \n");
	if (IsHead) {
		for (int i = 0; i < CLUSTER_NUM; i++) {
			std::cout << "\n  IP: " << Head[i].ip << ":" << Head[i].port << std::endl;
		}
	}
	else {
		std::cout << "\n  IP: " << Head_of_worker.ip << ":" << Head_of_worker.port << std::endl;
	}
	printf("\n  ******************************************** WORKER NODE ******************************************  \n");
	for (int i = 0; i < workernode; i++) {
		std::cout << "\n  IP: " << worker[i].ip << ":" << worker[i].port << std::endl;
	}
	printf("\n  ******************************************** ----------- ******************************************  \n");
}

char* Node::toString() {
	std::string res="";
	res.append("HN ");
	res.append(std::to_string(Node::headnode));
	for (int i = 0; i < headnode; i++) {
		res.append(" Head ");
		res.append(Node::Head[i].ip);
		res.append(" ");
		res.append(Node::Head[i].port);
		res.append(" ");
		res.append(std::to_string(Node::Worker_under_head[i]));
	}
	char *cstr = new char[res.length() + 1];
	strcpy(cstr, res.c_str());
	return cstr;
}

