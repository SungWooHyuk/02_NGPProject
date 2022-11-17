#pragma warning(disable : 4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지 
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>

#include "protocol.h"
#include "Server.h"

using namespace std;

#define SERVERPORT 9000
#define BUFSIZE 512
#define MAXCLIENT 3

HANDLE E_hThread[3]; // 이벤트 
HANDLE Ec_hThread[3]; // 이벤트 체크
HANDLE MyUpdateThread; // 이벤트 업데이트

HANDLE hThread; // 쓰레드 생성할 때 사용할 핸들
DWORD Threadid[3]; // 생성할 때 생기는 id값 담기
int p2;
int cnt{};
int Client_count = 0;
bool logincheck[3]{ false, false, false };
bool Initcheck = false;
LOGIN_PACKET login_info[3];
SOCKET client_sock[3];
INIT_PACKET init;
OBJECT_UPDATE_PACKET update_packet;

DWORD WINAPI Client_Thread(LPVOID arg);
DWORD WINAPI Update_Thread(LPVOID arg);

DWORD WINAPI Client_Thread(LPVOID arg)
{
	int m_id = Client_count;
	Client_count += 1;

	client_sock[m_id] = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen;
	//char buf[BUFSIZE];
	addrlen = sizeof(clientaddr);
	getpeername(client_sock[m_id], (SOCKADDR*)&clientaddr, &addrlen);

	if (logincheck[m_id] == false)
	{
		login_info[m_id].player.id = m_id;
		login_info[m_id].player.state_type = PLAYER::IDLE;
		login_info[m_id].player.x = 640;
		login_info[m_id].player.y = 360;
		login_info[m_id].player.visible = true;

		//send(client_sock, (char*)&login_info, sizeof(LOGIN_PACKET), 0);
		logincheck[m_id] = true;

	}

	if (Client_count == 1)
	{
		send(client_sock[m_id], (char*)&login_info[m_id], sizeof(LOGIN_PACKET), 0);
	}
	else if (Client_count == 2)
	{
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				send(client_sock[i], (char*)&login_info[j], sizeof(LOGIN_PACKET), 0);
			}
		}
	}
	else if (Client_count == 3)
	{
		for (int i = 0; i < Client_count; ++i)
		{
			for (int j = 0; j < Client_count; ++j)
			{
				send(client_sock[i], (char*)&login_info[j], sizeof(LOGIN_PACKET), 0);
			}
		}
	}

	// 리시브 후 센드 한번 해주고

	while (1) {
		WaitForSingleObject(Ec_hThread[m_id], INFINITE);

		SetEvent(E_hThread[m_id]);
		ResetEvent(Ec_hThread[m_id]);

	}
	return 0;
}

DWORD WINAPI Update_Thread(LPVOID arg)
{
	DWORD retval;
	printf("생성\n");
	while (1) {

		//WaitForSingleObject(hThread[0], INFINITE);

		retval = WaitForMultipleObjects(3, E_hThread, TRUE, INFINITE);

		if (Initcheck == false)
		{
			init.gameStart = true;
			init.timelap = 30;
			for (int i = 0; i < MAXCLIENT; ++i)
			{
				send(client_sock[i], (char*)&init, sizeof(init), 0);
			}
			Initcheck = true;
		}

		//send()
		//Update();
		//isCollision();
		//send;

		update_packet.gamemodestate = 0;
		update_packet.timelap -= 1;

		for (int i = 0; i < MAXCLIENT; ++i)
		{
			send(client_sock[i], (char*)&update_packet, sizeof(update_packet), 0);
		}

		ResetEvent(E_hThread[0]);
		ResetEvent(E_hThread[1]);
		ResetEvent(E_hThread[2]);
		SetEvent(Ec_hThread[0]);
		SetEvent(Ec_hThread[1]);
		SetEvent(Ec_hThread[2]);

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
	listen(listen_sock, SOMAXCONN); // 3명으로 제한

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE cli_Thread; // 접속할때마다 생성

	// 2번째 인자값 TRUE : 수동
	// 3번째 인자값 FALSE : 비신호 
	// 이벤트 신호 생성
	E_hThread[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
	E_hThread[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	E_hThread[2] = CreateEvent(NULL, TRUE, FALSE, NULL);
	Ec_hThread[0] = CreateEvent(NULL, TRUE, TRUE, NULL);
	Ec_hThread[1] = CreateEvent(NULL, TRUE, TRUE, NULL);
	Ec_hThread[2] = CreateEvent(NULL, TRUE, TRUE, NULL);
	MyUpdateThread = CreateEvent(NULL, TRUE, TRUE, NULL); //신호로 만들어줌  

	HANDLE update_Thread = CreateThread(NULL, 0, Update_Thread, NULL, 0, NULL); // 미리 생성

	while (1)
	{
		if (cnt < 3)
		{
			addrlen = sizeof(clientaddr);
			client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);


			cli_Thread = CreateThread(NULL, 0, Client_Thread, (LPVOID)client_sock, 0, &Threadid[cnt]);

			if (hThread == NULL) {
				
			}
			else {
				CloseHandle(hThread);
			}
			++cnt;
		}

	}

	closesocket(listen_sock);
	WSACleanup();

	// 이벤트 제거
	CloseHandle(hThread);
	CloseHandle(MyUpdateThread);

	return 0;
}