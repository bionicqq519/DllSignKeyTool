#pragma once
#include "Winsock.h"

class ServerEnd : public Winsock
{
public:
	ServerEnd();
	~ServerEnd();

	int SignKey();
	int RemoveFile(const char* a_cPath);
private:
	int EstablishServer();
};