
#include "./Winsock.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

Winsock::Winsock()
{
    int dErr;
    dErr = InitWinSock();
    return;
}

Winsock::~Winsock()
{
    WSACleanup();
    return;
}

int Winsock::InitWinSock()
{
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
    return 0;
}

int Winsock::ReleaseFile(unsigned char** ppucBuffer)
{
    free(*ppucBuffer);
    *ppucBuffer = nullptr;
    return 0;
}

int Winsock::ReadFile(const char* pcPath, unsigned char** ppucBuffer, int& dSize)
{
    struct stat t_File;
    stat(pcPath, &t_File);
    dSize = t_File.st_size;
    *ppucBuffer = (UINT8*)malloc(dSize * sizeof(UINT8));

    FILE* fp;
    if ((fp = fopen(pcPath, "rb")) != NULL)
    {
        fread(*ppucBuffer, sizeof(unsigned char), dSize, fp);
        fclose(fp);
        struct stat t;
        stat(pcPath, &t);
        //printf("[Read-size: %d] [Real-size: %d] [%s]\n", dFileSize, (int)t.st_size, pcPath);
    }
    else
    {
        printf("-Read-raw Fail.-%s-\n", pcPath);
        return 1;
    }
    return 0;
}

int Winsock::SendBuffer(unsigned char* a_ucSendBuf, int a_dBufLen)
{
    //send file info
    fileMessage t_fm = { 0 };
    strcpy(t_fm.fileName, "alPP_x64.dll");
    t_fm.fileSize = htonl(a_dBufLen);
    int iResult = send(m_Socket, (char*)&t_fm, sizeof(fileMessage), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(m_Socket);
        return 1;
    }
    //send file
    iResult = send(m_Socket, (char*)a_ucSendBuf, a_dBufLen, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(m_Socket);
        return 1;
    }
    return 0;
}

int Winsock::ReceiveFileAndSave(const char* a_cPath)
{
    fileMessage fileMsg = { 0 };
    if (recv(m_Socket, (char*)&fileMsg, sizeof(fileMsg), 0) <= 0) {
        printf("未接收到檔名與檔案長度，連線中斷... \n");
        closesocket(m_Socket);
        return 1;
    }

    char filename[50] = SAVE_PATH;
    //strcat(filename, fileMsg.fileName);
    strcpy(filename, a_cPath);

    FILE* fp = fopen(filename, "wb");
    if (NULL == fp)
    {
        printf("File: %s Can Not Open To Write\n", filename);
        system("pause");
        exit(1);
    }
    else
    {
        char buffer[DEFAULT_BUFLEN];
        memset(buffer, 0, DEFAULT_BUFLEN);
        int length = 0;
        int Total_length = ntohl(fileMsg.fileSize);

        do {
            length = recv(m_Socket, buffer, DEFAULT_BUFLEN, 0);
            if (fwrite(buffer, sizeof(char), length, fp) < length)
            {
                printf("File: %s Write Failed\n", filename);
                break;
            }
            memset(buffer, 0, DEFAULT_BUFLEN);
            Total_length -= length;
        } while (Total_length > 0 && length > 0);

        if (Total_length != 0)
        {
            printf("[RevBufAndDumpFromServer] size error\n");
        }

        printf("Receive File: Write %s From Server Successful!\n", filename);
    }

    fclose(fp);
    return 0;
}