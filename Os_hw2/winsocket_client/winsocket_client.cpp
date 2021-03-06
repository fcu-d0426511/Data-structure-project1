// winsocket_client.cpp: 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>


SOCKET sConnect;
DWORD WINAPI InputThread(LPVOID);

void main()
{
	char confirm;
	char recvMsg[200];
	char name[200];
	char sendbuf[200];
	bool index = true;
	char server_reply[200];
	DWORD ThreadID;
	HANDLE ThreadHandle;

	int r;
	//開始 Winsock-DLL
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData);

	//宣告給 socket 使用的 sockadder_in 結構
	SOCKADDR_IN addr;
	int addlen = sizeof(addr);

	//設定 socket
	sConnect = socket(AF_INET, SOCK_STREAM, NULL);

	//設定欲連線的Server的位址資訊
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);

	printf("connect to server?[Y] or [N] --> ");
	scanf_s("%c", &confirm,1);

	if (confirm == 'N')
	{
		exit(1);
	}
	else {
		if (confirm == 'Y')
		{
				printf("please input your name: ");
				scanf_s("%s",&name,sizeof(name));
				//printf("name%s", name);
				connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));
				send(sConnect, name, (int)strlen(name), 0);
				ZeroMemory(recvMsg, 200);
				r=recv(sConnect, recvMsg, sizeof(recvMsg), 0);

				printf("\n%s", recvMsg);
				//send(sConnect, name, (int)strlen(name), 0);
			//	memset(&sendbuf, 0, sizeof(sendbuf));
			//	memset(&recvMsg, 0, sizeof(recvMsg));

			ThreadHandle = CreateThread(NULL, 0, InputThread, 0, 0, &ThreadID);

			//接收 server 端的訊息
			do 
			{
			
			 	ZeroMemory(recvMsg, 200);
				recv(sConnect, recvMsg, sizeof(recvMsg), 0);
				if (strcmp(recvMsg, "exit") == 0) {
					break;
				}
				printf("%s\n", recvMsg);
			
			//	memset(&sendbuf, 0, sizeof(sendbuf));
			//	memset(&recvMsg, 0, sizeof(recvMsg));
				
			} while (index);
		}
	}
	
	closesocket(sConnect);
	WSACleanup();
	printf("bye bye...\n");
	system("pause");
}



DWORD WINAPI InputThread(LPVOID) { //接收 server 端的訊息                                                                                                            D0409929
	char sendMsg[200];
	printf("( Enter \"exit\" to close )\n");
	while (true) {
		ZeroMemory(sendMsg, 200);
		printf("\nenter Msg: \n");
		gets_s(sendMsg);
		send(sConnect, sendMsg, (int)strlen(sendMsg), 0);
		if (strcmp(sendMsg, "exit") == 0) {
			break;
		}
	}
	return 0;
}