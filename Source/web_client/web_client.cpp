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




int __cdecl main(int argc, char** argv)
{
    WSADATA wsaData;
    int iResult;
    
    
    // Validate the parameters
    if (argc < 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    
    fill_argv(argc, argv);

    get_link();
    form();
    
    SOCKET ConnectSocket = createSocket();
    

    iResult = 0;
    

    if (argc == 2) {
        // Send an initial buffer
        if (!is_folder()) {

            char* sendbuf = getSendbuf();

            iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }
        }


        // Download files
        if (is_folder()) iResult = downFolder(ConnectSocket);
        else iResult = downFile(ConnectSocket);
    }
    else {
        cout << "File's size: ";
        int k = 0;

        HANDLE* Array_Of_Thread_Handles = (HANDLE*)malloc((argc-1) * sizeof(HANDLE));
        HANDLE hThread;
        DWORD threadID;
        hThread = CreateThread(NULL, 0, threadFunc, &ConnectSocket, 0, &threadID);
        Array_Of_Thread_Handles[k] = hThread;
        k++;
        Sleep(600);

        for (int i = 2; i < argc; i++) {
            get_link();
            form();

            ConnectSocket = createSocket();
            hThread = CreateThread(NULL, 0, threadFunc, &ConnectSocket, 0, &threadID);
            Array_Of_Thread_Handles[k] = hThread;
            k++;
            Sleep(600);
        }
        WaitForMultipleObjects((argc-1), Array_Of_Thread_Handles, TRUE, INFINITE);
    }
    

    
    if (iResult > 0)
        printf("\nTotal File Bytes received: %d\n", iResult);
    else if (iResult == 0)
        printf("Connection closed\n");
    else
        printf("recv failed with error: %d\n", WSAGetLastError());



    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}