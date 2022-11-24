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
int cnt{};
int Client_count = 0;
bool logincheck[3]{ false, false, false };
bool Initcheck = false;

CRITICAL_SECTION cs;

LOGIN_PACKET login_info[3];
SOCKET client_sock[3];
INIT_PACKET init;
OBJECT_UPDATE_PACKET update_packet;
KEYINPUT_PAKCET key_input;
PLAYER::Player_State state_type;

void InitSettingObj();
void UpdatePlayer(short currentId);
void UpdateFire();
void UpdatePattern(short currentId);
void IsCollisionFloor(short currentId);
void IsCollisionThorn(short currentId);
void IsCollisionFire(short currentId);
void LoginDataSetting(int m_id);
void LoginSendPacket(int Client_count);

DWORD WINAPI Client_Thread(LPVOID arg);
DWORD WINAPI Update_Thread(LPVOID arg);

DWORD WINAPI Client_Thread(LPVOID arg)
{
	int m_id = Client_count;

	client_sock[m_id] = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen;
	//char buf[BUFSIZE];
	addrlen = sizeof(clientaddr);
	getpeername(client_sock[m_id], (SOCKADDR*)&clientaddr, &addrlen);

	//soo
	if (logincheck[m_id] == false)
	{
		LoginDataSetting(m_id);
	}

	LoginSendPacket(Client_count);

	while (1) {

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
			for (int i = 0; i < MAXCLIENT; ++i)
			{
				send(client_sock[i], (char*)&init, sizeof(init), 0);
			}
			Initcheck = true;
		}
		for (int i = 0; i < 3; ++i)
		UpdateFire();
			{
		for (int i = 0; i < FIRECNT; ++i)
		{
			update_packet.H_FireTemp[i].x = W_FireStatus[i].x;
			update_packet.W_FireTemp[i].y = H_FireStatus[i].y;
		}

		for (int i = 0; i < MAXCLIENT; ++i)
		{

			recv(client_sock[i], (char*)&key_input, sizeof(KEYINPUT_PAKCET), 0);
			playerStatus[key_input.m_id].state_type = key_input.state_type;
			playerStatus[key_input.m_id].jump = key_input.jump;

			EnterCriticalSection(&cs);
			UpdatePlayer(key_input.m_id);
			LeaveCriticalSection(&cs);
			update_packet.PlayerTemp[key_input.m_id].x = playerStatus[key_input.m_id].x;
			update_packet.PlayerTemp[key_input.m_id].y = playerStatus[key_input.m_id].y;
			UpdatePlayer(i);  
			send(client_sock[i], (char*)&update_packet, sizeof(update_packet), 0);
		}
	{
		ResetEvent(E_hThread[0]);
		ResetEvent(E_hThread[1]);
		ResetEvent(E_hThread[2]);
		SetEvent(Ec_hThread[0]);
		SetEvent(Ec_hThread[1]);
		SetEvent(Ec_hThread[2]);
	}
	return 0;
}
		//fire 업데이트 부분 
		UpdateFire();
	else if (Client_count == 3)
		
		
		for (int i = 0; i < MAXCLIENT; ++i) {
			//여기에선 clientThread에서 키값 입력 받고 플레이어3명의 위치 모두 바꿔주었기 때문에 충돌 여부 확인 가능
			IsCollisionFloor(i);
			IsCollisionThorn(i);
			IsCollisionFire(i);
			//pattern 업데이트 부분 -> 여기서 진행 
			UpdatePattern(i); //충돌처리랑 위치 이동 모두 여기서 할것임 
		}
		
		//send;

		update_packet.gamemodestate = 0;
		update_packet.timelap -= 1;
		//수행하고
		for (int i = 0; i < MAXCLIENT; ++i)
		{
			send(client_sock[i], (char*)&update_packet, sizeof(update_packet), 0);
		}
		ResetEvent(Ec_hThread[m_id]);

	}
	return 0;
}
			for (int i = 0; i < MAXCLIENT; ++i)
			{
				send(client_sock[i], (char*)&init, sizeof(init), 0);
			}
			Initcheck = true;
		}

		UpdateFire();
		while ((GetTickCount64() - StartTime) <= 10) {
		for (int i = 0; i < FIRECNT; ++i)
		{
			update_packet.H_FireTemp[i].x = W_FireStatus[i].x;
			update_packet.W_FireTemp[i].y = H_FireStatus[i].y;
		}

		for (int i = 0; i < MAXCLIENT; ++i)
		{

			cli_Thread = CreateThread(NULL, 0, Client_Thread, (LPVOID)client_sock, 0, 0);
			UpdatePlayer(key_input.m_id);
			LeaveCriticalSection(&cs);
			update_packet.PlayerTemp[key_input.m_id].x = playerStatus[key_input.m_id].x;
			update_packet.PlayerTemp[key_input.m_id].y = playerStatus[key_input.m_id].y;
			for (int i = 0; i < BUTTONCNT; ++i)
			send(client_sock[i], (char*)&update_packet, sizeof(update_packet), 0);
		}
				init.button[i].y = ButtonStatus[i].y;
				init.button[i].x_size = ButtonStatus[i].x_size;
				init.button[i].y_size = ButtonStatus[i].y_size;
			}

			for (int i = 0; i < MAXCLIENT; ++i)
			{
				send(client_sock[i], (char*)&init, sizeof(init), 0);
			}
			Initcheck = true;
		}

		UpdateFire();

		for (int i = 0; i < FIRECNT; ++i)
		{
			update_packet.H_FireTemp[i].x = W_FireStatus[i].x;
			update_packet.W_FireTemp[i].y = H_FireStatus[i].y;
		}

		for (int i = 0; i < MAXCLIENT; ++i)
		{



			cli_Thread = CreateThread(NULL, 0, Client_Thread, (LPVOID)client_sock, 0, &Threadid[cnt]);

			if (hThread == NULL) {
			playerStatus[key_input.m_id].state_type = key_input.state_type;
			playerStatus[key_input.m_id].jump = key_input.jump;

			EnterCriticalSection(&cs);
			UpdatePlayer(key_input.m_id);
			LeaveCriticalSection(&cs);
			update_packet.PlayerTemp[key_input.m_id].x = playerStatus[key_input.m_id].x;
			update_packet.PlayerTemp[key_input.m_id].y = playerStatus[key_input.m_id].y;

			send(client_sock[i], (char*)&update_packet, sizeof(update_packet), 0);
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
}

int main(int argc, char* argv[])
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN serveraddr;
			cli_Thread = CreateThread(NULL, 0, Client_Thread, (LPVOID)client_sock, 0, 0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	listen(listen_sock, SOMAXCONN); // 3명으로 제한

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE cli_Thread; // 접속할때마다 생성
	InitializeCriticalSection(&cs);
	InitSettingObj();
	// 2번째 인자값 TRUE : 수동
	// 3번째 인자값 FALSE : 비신호 
	// 이벤트 신호 생성
	E_hThread[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
	Ec_hThread[0] = CreateEvent(NULL, TRUE, TRUE, NULL);
	
	E_hThread[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	Ec_hThread[1] = CreateEvent(NULL, TRUE, TRUE, NULL);
	
	E_hThread[2] = CreateEvent(NULL, TRUE, FALSE, NULL);
	Ec_hThread[2] = CreateEvent(NULL, TRUE, TRUE, NULL);

	MyUpdateThread = CreateEvent(NULL, TRUE, TRUE, NULL); //신호로 만들어줌  

	HANDLE update_Thread = CreateThread(NULL, 0, Update_Thread, NULL, 0, NULL); // 미리 생성

	while (1)
	{
		if (cnt < 3)
		{
			addrlen = sizeof(clientaddr);
			client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
			cli_Thread = CreateThread(NULL, 0, Client_Thread, (LPVOID)client_sock, 0, 0);

			++cnt;
		}

	}

	DeleteCriticalSection(&cs);
	closesocket(listen_sock);
	WSACleanup();

	// 이벤트 제거
	CloseHandle(cli_Thread);
	CloseHandle(MyUpdateThread);

	return 0;
}


void UpdatePlayer(short currentId) {
	//키 값에 따라 player의 위치 업데이트해주어야 함 
	//콜리젼 박스도 해야함 
	switch (playerStatus[currentId].state_type) {
	case PLAYER::Player_State::LEFT:
		moveSpeed[currentId] = -3.f;
		break;
	case PLAYER::Player_State::RIGHT:
		moveSpeed[currentId] = 3.f;
		break;
	case PLAYER::Player_State::IDLE:
		moveSpeed[currentId] = 0.f;
		break;
	case PLAYER::Player_State::DEAD:
		break;
	}
	
	if (playerStatus[currentId].jump && (jumpCnt[currentId]<2))
	{
		dropSpeed[currentId] = -6.f;
		if (jumpCnt[currentId] < 2 )
			jumpCnt[currentId]++; 
	}
	else 
		dropSpeed[currentId] += gravity;
	
	if (dropSpeed[currentId] >= 0.f)
		IsCollisionFloor(currentId);
	

	playerStatus[currentId].x += moveSpeed[currentId];
	playerStatus[currentId].y += dropSpeed[currentId]; // 여기서 y이동
	playerStatus[currentId].CollidBox = RECT_OBJECT(playerStatus[currentId].x, playerStatus[currentId].y, 24, 28); // 콜리젼박스 업데이트
}

void UpdateFire() {
	//불 사라지면 다시 위치 셋팅 
	for (int i = 0; i < FIRECNT; ++i) {
		// 초기화
		if (H_FireStatus[i].y > 750.f)
		{
			switch (i) {
			case 0:
				H_FireStatus[i].x = 70;
				break;
			case 1:
				H_FireStatus[i].x = 380;
				break;
			case 2:
				H_FireStatus[i].x = 500;
				break;
			case 3:
				H_FireStatus[i].x = 710;
				break;
			case 4:
				H_FireStatus[i].x = 935;
				break;
			}
			H_FireStatus[i].y = -30.f;
			H_FiredropSpeed[i] = 0.f;
		}
		if (W_FireStatus[i].x > 1300.f)
		{
			switch (i) {
			case 0:
				W_FireStatus[i].y = 47;
				break;
			case 1:
				W_FireStatus[i].y = 218;
				break;
			case 2:
				W_FireStatus[i].y = 317;
				break;
			case 3:
				W_FireStatus[i].y = 482;
				break;
			case 4:
				W_FireStatus[i].y = 573;
				break;
			}
			W_FireStatus[i].x = -30.f;
			W_FiredropSpeed[i] = 0.f;
		}

		W_FiredropSpeed[i] += W_Firegravity[i]; // 스피드 더해주고
		H_FiredropSpeed[i] += H_Firegravity[i];

		W_FireStatus[i].x += W_FiredropSpeed[i]; // 상태 변화
		H_FireStatus[i].y += H_FiredropSpeed[i]; // 상태 변화

		//콜리젼 박스 업데이트 
		//FireStatus[i].CollidBox = RECT_OBJECT(FireStatus[i].x, FireStatus[i].y, FireStatus[i].x_size, FireStatus[i].y_size);
		//W_FireStatus[i].CollidBox = RECT_OBJECT(W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size);
	}
}
void UpdatePattern(short currentId) {
	for (int i = 0; i < PATTERNCNT; ++i) {
		if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &PatternStatus[i].CollidBox))
		{
			PatternStatus[i].x = 1200 - 30 * i; //충돌하면 우측 상단으로 이동시키는 코드
			PatternStatus[i].y = 0;
			isPatternClear[i] = true;
		}

		//5개 모두 획득했다면 
		for (int i = 0; i < PATTERNCNT; ++i) {
			if (isPatternClear[i] == true)
				check += 1;
			if (check == 5)
				update_packet.gamemodestate = 2; //gameClear 의미함 
		}
	}
}

void IsCollisionFloor(short currentId) {
	RECT PlayerBottomBox = playerStatus[currentId].CollidBox;
	PlayerBottomBox.top += 20;
	PlayerBottomBox.right -= 10;
	PlayerBottomBox.left += 10;
	for (int i = 0; i < FLOORCNT; ++i)
	{
		if (IntersectRect(&rt, &PlayerBottomBox, &floorStatus[i].CollidBox))
		{
			playerStatus[currentId].y = floorStatus[i].y - 27;
			playerStatus[currentId].jump = false;
			dropSpeed[currentId] = 0.f;
			jumpCnt[currentId] = 0;
		}
	}
}

void IsCollisionThorn(short currentId) {
	for (int i = 0; i < THORNCNT; ++i) // 가시에 닿는 충돌체크 아래 일단 주석.
	{
		if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &ThornStatus[i].CollidBox))
		{
			update_packet.gamemodestate = 0;
			update_packet.PlayerTemp[currentId].state_type = PLAYER::Player_State::DEAD;
		}
	}
}

void IsCollisionFire(short currentId) {
	for (int i = 0; i < FIRECNT; ++i) // 불이랑 체크
	{
		if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &H_FireStatus[i].CollidBox))
		{
			update_packet.gamemodestate = 0;
			update_packet.PlayerTemp[currentId].state_type = PLAYER::Player_State::DEAD;
		}

		if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &W_FireStatus[i].CollidBox))
		{
			update_packet.gamemodestate = 0;
			update_packet.PlayerTemp[currentId].state_type = PLAYER::Player_State::DEAD;
		}
	}
}

void InitSettingObj() {
	random_device rd; // 시드값
	default_random_engine dre{ rd() }; // 초기 조건은 추적 불가능하다.
	uniform_int_distribution<int> choice_uid{ 0,9 }; // 문양 위치 랜덤값  
	// 바닥 위치 셋팅
	floorStatus[0] = Object(0, 120, 170, 158, 30);
	floorStatus[1] = Object(1, 100, 550, 158, 30);
	update_packet.PlayerTemp[m_id].id = m_id;
	update_packet.PlayerTemp[m_id].x = playerStatus[m_id].x;
	update_packet.PlayerTemp[m_id].y = playerStatus[m_id].y;
	update_packet.PlayerTemp[m_id].x_size = 24;
	update_packet.PlayerTemp[m_id].y_size = 28;
	update_packet.PlayerTemp[m_id].state_type = playerStatus[m_id].state_type;
	update_packet.PlayerTemp[m_id].visible = true;
	floorStatus[3] = Object(3, 360, 510, 158, 30);
	playerStatus[m_id].CollidBox = RECT_OBJECT(playerStatus[m_id].x, playerStatus[m_id].y, 24, 28);
	floorStatus[7] = Object(7, 850, 510, 158, 30);
	logincheck[m_id] = true;
}
	floorStatus[9] = Object(9, 1050, 600, 158, 30);
void LoginSendPacket(int Client_count)
{
	if (Client_count == 1)
	{
		send(client_sock[0], (char*)&login_info[0], sizeof(LOGIN_PACKET), 0);
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
				login_info[j].cli_id = i;
				send(client_sock[i], (char*)&login_info[j], sizeof(LOGIN_PACKET), 0);
			}
		}
	ThornStatus[3] = Object(3, 435, 630, 145, 55);
	ThornStatus[4] = Object(4, 580, 630, 145, 55);
	ThornStatus[5] = Object(5, 725, 630, 145, 55);
	ThornStatus[6] = Object(6, 870, 630, 145, 55);
	ThornStatus[7] = Object(7, 1015, 630, 145, 55);
	ThornStatus[8] = Object(8, 1160, 630, 145, 55);
	//고정
	for (int i = 0; i < THORNCNT; ++i) {
		ThornStatus[i].CollidBox = RECT_OBJECT(ThornStatus[i].x, ThornStatus[i].y, ThornStatus[i].x_size, ThornStatus[i].y_size);
	}

	// 세로 불 초기값
	H_FireStatus[0] = Object(0, 70, -30, 34, 51);
	H_FireStatus[1] = Object(1, 380, -30, 34, 51);
	H_FireStatus[2] = Object(2, 500, -30, 34, 51);
	H_FireStatus[3] = Object(3, 710, -30, 34, 51);
	H_FireStatus[4] = Object(4, 935, -30, 34, 51);
	for (int i = 0; i < FIRECNT; ++i) {
void FPSCheck() {
	W_FireStatus[3] = Object(3, -30, 482, 51, 34);
	DWORD currTime = timeGetTime();
	frameDelta = (currTime - lastTime) * 0.001f;
	DWORD FPS = 15;                     //15(FPS)라는 숫자가 fps를 결정한다.숫자가 작을수록 더 많은 프레임을 그린다..
	for (int i = 0; i < FIRECNT; ++i) {
	//컴퓨터 성능에 따라 테스트하여가장 느린 fps를 검출한 후 결정한다.

	if (frameDelta >= 1 / FPS)
	{

		//렌더링 하는 부분.
		lastTime = currTime;
	}
	else {
		//Sleep(frameDelta - 1 / FPS); //넣어줘도 되고 그냥 주석 처리해도 상관 없음니다.

	update_packet.PlayerTemp[m_id].y_size = 28;
	update_packet.PlayerTemp[m_id].state_type = playerStatus[m_id].state_type;
	update_packet.PlayerTemp[m_id].visible = true;
		PatternStatus[i] = Object(i, floorStatus[k].x + floorStatus[k].x_size / 3, floorStatus[k].y - floorStatus[k].y_size - floorStatus[k].y_size / 2, 50, 50); // 뒤에 + 로 중앙으로
	playerStatus[m_id].CollidBox = RECT_OBJECT(playerStatus[m_id].x, playerStatus[m_id].y, 24, 28);
		PatternStatus[i].CollidBox = RECT_OBJECT(PatternStatus[i].x, PatternStatus[i].y, PatternStatus[i].x_size, PatternStatus[i].y_size);
	logincheck[m_id] = true;
}
	//버튼 초기 위치 셋팅 - 이건 정확한 값 아니고 노가다로 한번 위치 맞춰봐야함 
void LoginSendPacket(int Client_count)
{
	if (Client_count == 1)
	{
		send(client_sock[0], (char*)&login_info[0], sizeof(LOGIN_PACKET), 0);
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
				login_info[j].cli_id = i;
				send(client_sock[i], (char*)&login_info[j], sizeof(LOGIN_PACKET), 0);
			}
		}
	update_packet.PlayerTemp[m_id].y_size = 28;
	update_packet.PlayerTemp[m_id].state_type = playerStatus[m_id].state_type;
	update_packet.PlayerTemp[m_id].visible = true;

	playerStatus[m_id].CollidBox = RECT_OBJECT(playerStatus[m_id].x, playerStatus[m_id].y, 24, 28);

	logincheck[m_id] = true;
}

void LoginSendPacket(int Client_count)
{
	if (Client_count == 1)
	{
		send(client_sock[0], (char*)&login_info[0], sizeof(LOGIN_PACKET), 0);
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
				login_info[j].cli_id = i;
				send(client_sock[i], (char*)&login_info[j], sizeof(LOGIN_PACKET), 0);
			}
		}
	}
}