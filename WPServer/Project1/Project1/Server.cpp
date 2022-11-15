#pragma comment(lib, "ws2_32")
#pragma warning(disable : 4996)
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>

#define SERVERPORT 9000
#define BUFSIZE 4096
#define MAXCLIENT 3

DWORD Threadid[3];
HANDLE hThread[3];
HANDLE MyUpdateThread;
HANDLE hhThread[3];

int cnt{};

DWORD WINAPI Client_Thread(LPVOID arg)
{

	int m_id = (int)arg;

	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE];
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	// 리시브 후 센드 한번 해주고

	while (1) {

		WaitForSingleObject(hhThread[m_id], INFINITE);
		printf("안녕하세요 %d번\n", m_id);

		SetEvent(hThread[m_id]);
		ResetEvent(hhThread[m_id]);

	}
	return 0;
}

DWORD WINAPI Update_Thread(LPVOID arg)
{
	DWORD retval;
	//printf("생성\n");
	while (1) {

		//WaitForSingleObject(hThread[0], INFINITE);

		retval = WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

		//Update();
		//isCollision();
		//send;

		printf("업데이트\n");

		ResetEvent(hThread[0]);
		ResetEvent(hThread[1]);
		ResetEvent(hThread[2]);
		SetEvent(hhThread[0]);
		SetEvent(hhThread[1]);
		SetEvent(hhThread[2]);

		//ResetEvent(MyUpdateThread);
	}
	return 0;
}
int main(int argc, char* argv[])
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	listen(listen_sock, 3); // 3명으로 제한

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE cli_Thread; // 접속할때마다 생성

	// 2번째 인자값 TRUE : 수동
	// 3번째 인자값 FALSE : 비신호 
	// 이벤트 신호 생성
	hThread[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
	hThread[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	hThread[2] = CreateEvent(NULL, TRUE, FALSE, NULL);
	hhThread[0] = CreateEvent(NULL, TRUE, TRUE, NULL);
	hhThread[1] = CreateEvent(NULL, TRUE, TRUE, NULL);
	hhThread[2] = CreateEvent(NULL, TRUE, TRUE, NULL);
	MyUpdateThread = CreateEvent(NULL, TRUE, TRUE, NULL); //신호로 만들어줌  

	HANDLE update_Thread = CreateThread(NULL, 0, Update_Thread, NULL, 0, NULL); // 미리 생성
	while (1)
	{

		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);

		cli_Thread = CreateThread(NULL, 0, Client_Thread, (LPVOID)cnt, 0, &Threadid[cnt]);

		if (hThread == NULL) {
			closesocket(client_sock);
		}
		else {
			CloseHandle(hThread);
		}

		++cnt;

	}

	closesocket(listen_sock);
	WSACleanup();

	// 이벤트 제거
	CloseHandle(hThread);
	CloseHandle(MyUpdateThread);

	return 0;
}