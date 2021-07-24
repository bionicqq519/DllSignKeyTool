#include "Winsock.h"
#include "Client.h"
#include "Server.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include "time.h"

using namespace std;

int main(int argc, char* argv[])
{
	ClientEnd* ptClient = nullptr;
	ServerEnd* ptServer = nullptr;

	printf("DLL sign key tool v0.00.01 start ...\n");
	printf("(1) sign key server\n");
	printf("(2) §Ú­nsign key\n");
	int dClientOrServer = 0;
	scanf("%d", &dClientOrServer);

	if(dClientOrServer==2)	//Client
	{
		char setIP[50] = "localhost"; // 127.0.0.1

		if (argc >= 2)
		{
			strcpy(setIP, argv[1]);
		}
		else
		{
			printf("Please enter the server IP:\n");
			scanf(" %s", setIP);
			printf("set IP : %s\n", setIP);
		}

		ptClient = new ClientEnd();

		int connect = 1;
		do {
			connect = ptClient->ClientConnectToServer(setIP);
			Sleep(100);
		} while (connect==1);

		printf("Connect to Server success!\n");

		//printf("sign key y/n\n");
		//char dSignKeyQ = 'y';
		//scanf(" %c", &dSignKeyQ);
		//while(dSignKeyQ == 'y')
		{
			//1.read dll file for sign key
			const char* pucReadPath = ".\\alPP_x64.dll";
			unsigned char* pucDllBuf = nullptr;
			int dDllSize = 0;
			ptClient->ReadFile(pucReadPath, &pucDllBuf, dDllSize);
			//2.send dll to Server
			ptClient->SendBuffer(pucDllBuf, dDllSize);
			//3.release send data resource
			ptClient->ReleaseFile(&pucDllBuf);
			//4.receive sign key library
			const char* pucSavePath = ".\\alPP_x64_sign.dll";
			ptClient->ReceiveFileAndSave(pucSavePath);

			//scanf(" %c", &dSignKeyQ);
		}
	}
	else if (dClientOrServer == 1)	//Server
	{
		while (true)
		{
			printf("sign key server on...\n");
			ptServer = new ServerEnd();
			const char* pucPath = SAVE_PATH "alPP_x64.dll";
			//1.get dll from server
			ptServer->ReceiveFileAndSave(pucPath);
			//2.sign dll
			ptServer->SignKey();
			//3.read signed dll for transfer
			unsigned char* pucDllBuf = nullptr;
			int dDllSize = 0;
			ptServer->ReadFile(pucPath, &pucDllBuf, dDllSize);
			//4. send signed dll to Client
			ptServer->SendBuffer(pucDllBuf, dDllSize);
			//5. release send data resource
			ptServer->ReleaseFile(&pucDllBuf);
			//6. remove save data
			ptServer->RemoveFile(pucPath);

			if (!ptServer) {
				delete ptServer;
				ptServer = nullptr;
			}
		}
	}
	

	if (!ptClient) delete ptClient;
	if (!ptServer) delete ptServer;

	printf("DLL sign key tool end ...\n");
	system("pause");
	return 0;
}