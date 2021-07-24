#include "./Client.h"
#include <stdio.h>
#include <sys/stat.h>

ClientEnd::ClientEnd()
{
    //int dErr;
    return;
}

ClientEnd::~ClientEnd()
{
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // shutdown the connection since no more data will be sent
    int iResult = shutdown(m_Socket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(m_Socket);
        return;
    }

    // Receive until the peer closes the connection
    do {
        iResult = recv(m_Socket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);

    // cleanup
    closesocket(m_Socket);
    return;
}

int ClientEnd::ClientConnectToServer(const char* pcServerIP)
{
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    int iResult = getaddrinfo(pcServerIP, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        m_Socket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (m_Socket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            return 1;
        }

        // Connect to server.
        iResult = connect(m_Socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(m_Socket);
            m_Socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (m_Socket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        return 1;
    }

    return 0;
}