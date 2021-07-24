#pragma once
#include "./Winsock.h"
class ClientEnd : public Winsock
{
public:
	ClientEnd();
	~ClientEnd();

	int ClientConnectToServer(const char* pcServerIP);

	

private:

};