#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "88888"

#define WIN32_LEAN_AND_MEAN

#define SAVE_PATH ".//"

#pragma pack(push, 1)
typedef struct {
	char fileName[256];
	long int fileSize;
}fileMessage;
#pragma pack(pop)

class Winsock
{
	
public:
	Winsock();
	~Winsock();

	int ReadFile(const char* pcPath, unsigned char** ppucBuffer, int& dSize);
	int ReleaseFile(unsigned char** ppucBuffer);

	int SendBuffer(unsigned char* a_ucSendBuf, int a_dBufLen);
	int ReceiveFileAndSave(const char* a_cPath);
protected:
	
	SOCKET m_Socket;
private:
	int InitWinSock();

	WSADATA m_wsaData;

};