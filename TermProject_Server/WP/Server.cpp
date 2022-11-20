#pragma warning(disable : 4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지 
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "winmm.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <random>
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
int ccnt{};
int Client_count = 0;
bool logincheck[3]{ false, false, false };
bool Initcheck = false;
LOGIN_PACKET login_info[3];
SOCKET client_sock[3];
INIT_PACKET init;
OBJECT_UPDATE_PACKET update_packet;
//시간

static DWORD frameDelta = 0;
static DWORD lastTime = timeGetTime();
//무한 루프 

void InitSettingObj();
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
		login_info[m_id].player.x = 640 + (Client_count * 20);
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
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
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

	while (1) {

		retval = WaitForMultipleObjects(3, E_hThread, TRUE, INFINITE);


		if (Initcheck == false)
		{
			init.gameStart = true;
			init.timelap = 50;
			for (int i = 0; i < PATTERNCNT; ++i)
			{
				init.pattern_temp[i].x = PatternStatus[i].x;
				init.pattern_temp[i].y = PatternStatus[i].y;
				init.pattern_temp[i].x_size = PatternStatus[i].x_size;
				init.pattern_temp[i].y_size = PatternStatus[i].y_size;

			}
			for (int i = 0; i < BUTTONCNT; ++i)
			{
				init.button[i].x = ButtonStatus[i].x;
				init.button[i].y = ButtonStatus[i].y;
				init.button[i].x_size = ButtonStatus[i].x_size;
				init.button[i].y_size = ButtonStatus[i].y_size;
			}

			for (int i = 0; i < MAXCLIENT; ++i)
			{
				send(client_sock[i], (char*)&init, sizeof(INIT_PACKET), 0);
			}
			Initcheck = true;
		}

		ResetEvent(E_hThread[0]);
		ResetEvent(E_hThread[1]);
		ResetEvent(E_hThread[2]);
		SetEvent(Ec_hThread[0]);
		SetEvent(Ec_hThread[1]);
		SetEvent(Ec_hThread[2]);
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

	InitSettingObj();
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

void InitSettingObj() {
	random_device rd; // 시드값
	default_random_engine dre{ rd() }; // 초기 조건은 추적 불가능하다.
	uniform_int_distribution<int> choice_uid{ 0,9 }; // 문양 위치 랜덤값  
	// 바닥 위치 셋팅
	floorStatus[0] = Object(0, 120, 170, 158, 30);
	floorStatus[1] = Object(1, 100, 550, 158, 30);
	floorStatus[2] = Object(2, 370, 280, 158, 30);
	floorStatus[3] = Object(3, 360, 510, 158, 30);
	floorStatus[4] = Object(4, 640, 390, 158, 30);
	floorStatus[5] = Object(5, 600, 600, 158, 30);
	floorStatus[6] = Object(6, 820, 280, 158, 30);
	floorStatus[7] = Object(7, 850, 510, 158, 30);
	floorStatus[8] = Object(8, 1030, 140, 158, 30);
	floorStatus[9] = Object(9, 1050, 600, 158, 30);

	// 첫 가시 위치 바뀔일 없음 
	ThornStatus[0] = Object(0, 0, 630, 145, 55);
	ThornStatus[1] = Object(1, 145, 630, 145, 55);
	ThornStatus[2] = Object(2, 290, 630, 145, 55);
	ThornStatus[3] = Object(3, 435, 630, 145, 55);
	ThornStatus[4] = Object(4, 580, 630, 145, 55);
	ThornStatus[5] = Object(5, 725, 630, 145, 55);
	ThornStatus[6] = Object(6, 870, 630, 145, 55);
	ThornStatus[7] = Object(7, 1015, 630, 145, 55);
	ThornStatus[8] = Object(8, 1160, 630, 145, 55);

	// 세로 불 초기값
	FireStatus[0] = Object(0, 70, -30, 34, 51);
	FireStatus[1] = Object(1, 380, -30, 34, 51);
	FireStatus[2] = Object(2, 500, -30, 34, 51);
	FireStatus[3] = Object(3, 710, -30, 34, 51);
	FireStatus[4] = Object(4, 935, -30, 34, 51);

	// 가로 불 초기값
	W_FireStatus[0] = Object(0, -30, 47, 51, 34);
	W_FireStatus[1] = Object(1, -30, 218, 51, 34);
	W_FireStatus[2] = Object(2, -30, 317, 51, 34);
	W_FireStatus[3] = Object(3, -30, 482, 51, 34);
	W_FireStatus[4] = Object(4, -30, 573, 51, 34);


	// 흑백 패턴 초기값 셋팅
	gs_PatternStatus[0] = Object(0, 1200, 0, 50, 50);
	gs_PatternStatus[1] = Object(1, 1170, 0, 50, 50);
	gs_PatternStatus[2] = Object(2, 1140, 0, 50, 50);
	gs_PatternStatus[3] = Object(3, 1110, 0, 50, 50);
	gs_PatternStatus[4] = Object(4, 1080, 0, 50, 50);

	// 문 초기값 셋팅 
	doorstatus = Object(0, 723, 380, 1, 1); //이건 문양 다 먹으면 서버쪽에서 값 계산해서 클라로 보내주면 됨



	// 패턴의 초기 위치 셋팅
	for (int i = 0; i < PATTERNCNT; ++i)
	{
		int k = choice_uid(dre); // 랜덤값인데 안겹치게 체크 
		while (1)
		{
			if (randomCheck[k] == false) {
				randomCheck[k] = true;
				break;
			}
			else if (randomCheck[k])
				k = choice_uid(dre); //이미 true인 곳은 다시 랜덤값 주기 
		}
		PatternStatus[i] = Object(i, floorStatus[k].x + floorStatus[k].x_size / 3, floorStatus[k].y - floorStatus[k].y_size - floorStatus[k].y_size / 2, 50, 50); // 뒤에 + 로 중앙으로

	}

	//버튼 초기 위치 셋팅 - 이건 정확한 값 아니고 노가다로 한번 위치 맞춰봐야함 
	ButtonStatus[0] = Object(0, 100, 530, 30, 20);
	ButtonStatus[1] = Object(1, 600, 580, 30, 20);


}

void FPSCheck() {

	DWORD currTime = timeGetTime();
	frameDelta = (currTime - lastTime) * 0.001f;
	DWORD FPS = 15;                     //15(FPS)라는 숫자가 fps를 결정한다.숫자가 작을수록 더 많은 프레임을 그린다..

	//컴퓨터 성능에 따라 테스트하여가장 느린 fps를 검출한 후 결정한다.

	if (frameDelta >= 1 / FPS)
	{

		//렌더링 하는 부분.
		lastTime = currTime;
	}
	else {
		//Sleep(frameDelta - 1 / FPS); //넣어줘도 되고 그냥 주석 처리해도 상관 없음니다.

	}
}