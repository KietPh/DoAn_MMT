// web_client.cpp : Defines the entry point for the console application.
//


// Ref: https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code

#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")


#define DEFAULT_BUFLEN 10000
#define DEFAULT_PORT "80"

int __cdecl main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    const char* sendbuf = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
   
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
    
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    
    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    
    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    
    printf("Bytes Sent: %ld\n", iResult);



    // Receive until the peer closes the connection
    int byterecv, len, data_len, size_len_head, size_len_tail = 0, hSz = 0;
    iResult = 0;
    ofstream of;
    of.open("1.html", ios::binary | ios::app);

    do {
        memset(recvbuf, 0, recvbuflen);
        byterecv = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (byterecv == 0) break;
        size_len_head = 0;
        size_len_tail = 2;
        if (iResult == 0) hSz = headerSz(recvbuf);
        if (recvbuf[0] == '0' && recvbuf[1] == '\r' && recvbuf[2] == '\n') break;
        data_len = strlen(recvbuf + hSz);
        for (int i = 0; i < data_len; i++) {
            if ((recvbuf + hSz)[i] == '\r' && (recvbuf + hSz)[i + 1] == '\n') {
                size_len_head = i + 2;
                break;
            }
        }


        if (iResult == 0) {
            //printf("Data:\n%s", recvbuf);
            of.write(recvbuf + hSz + size_len_head, byterecv - hSz - size_len_tail - size_len_head);
            hSz = 0;
        }
        else {
            of.write(recvbuf + size_len_head, byterecv - size_len_tail - size_len_head);
        }
        iResult += byterecv;
    } while (byterecv > 0);
    of.close();



    if (iResult > 0)
        printf("\nTotal Bytes received: %d\n", iResult);
    else if (iResult == 0)
        printf("Connection closed\n");
    else
        printf("recv failed with error: %d\n", WSAGetLastError());


    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
