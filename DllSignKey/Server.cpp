
#include "./Server.h"
#include <stdio.h>
#include <sys/stat.h>
#include <direct.h> // _getcwd

ServerEnd::ServerEnd()
{
    int dErr;
    EstablishServer();
    return;
}

ServerEnd::~ServerEnd()
{
    // cleanup
    closesocket(m_Socket);
    return;
}

int ServerEnd::RemoveFile(const char* a_cPath)
{
    // 0:жие\ 1:ев▒╤
    return remove(a_cPath);
}


int ServerEnd::SignKey()
{
    system("set WORKPATH=\"C:\\Program Files(x86)\\Windows Kits\\10\\bin\\10.0.19041.0\\x64\"");
    char cCurPath[256];
    _getcwd(cCurPath, 256);
    char signKeyCmd[1024] = "\0";
    strcat(signKeyCmd, "call signtool sign /tr http://timestamp.digicert.com /td sha256 /fd sha256 /a \"");
    strcat(signKeyCmd, cCurPath);
    strcat(signKeyCmd, "\\alPP_x64.dll\"");
    printf("sign cmd: %s\n", signKeyCmd);
    system(signKeyCmd);
    return 0;
}

int ServerEnd::EstablishServer()
{
    SOCKET ListenSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    int iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        return 1;
    }

    freeaddrinfo(result);
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        return 1;
    }
    
    // Accept a client socket
    m_Socket = accept(ListenSocket, NULL, NULL);
    if (m_Socket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);
    return 0;
}