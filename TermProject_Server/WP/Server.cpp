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

bool firstbutton[3] = { false,false,false };
bool secondbutton[3] = { false,false,false };
bool doorcheck[3] = { false, false, false };

string nickname[3];

HANDLE E_hThread[3]; // 이벤트 
HANDLE hThread; // 쓰레드 생성할 때 사용할 핸들

CRITICAL_SECTION cs;

LOGIN_PACKET first_login[3]; 
LOGIN_PACKET login_info[3];
INIT_PACKET init;
OBJECT_UPDATE_PACKET update_packet;
KEYINPUT_PAKCET key_input;

SOCKET client_sock[3];
PLAYER::Player_State state_type;

void InitSettingObj();

void UpdatePlayer(short currentId);
void UpdateFire();
void UpdatePattern();
void UpdateButton();
void UpdateDoor();

void IsCollisionFloor(short currentId);
void IsCollisionThorn(short currentId);
void IsCollisionFire(short currentId);
void IsCollisionPattern(short currentId); 
void IsCollisionButton(short currentId);
void IsCollisionDoor(short currentId);
void UpdateCollisionPackage(short currentId); 

void LoginDataSetting(int currentId);
void LoginSendPacket(int Client_count);

void PlayerReset(short currentId);
void UpdateFPS();

DWORD WINAPI Client_Thread(LPVOID arg);
DWORD WINAPI Update_Thread(LPVOID arg);

DWORD WINAPI Client_Thread(LPVOID arg)
{
	int m_id = Client_count;

	Client_count++;

	client_sock[m_id] = (SOCKET)arg;

	cout << m_id+1 << "번째 클라이언트 접속" << endl;
	
	if (logincheck[m_id] == false)
	{
		LoginDataSetting(m_id);
	}

	LoginSendPacket(Client_count);

	SetEvent(E_hThread[m_id]);
	
	return 0;
}

DWORD WINAPI Update_Thread(LPVOID arg)
{
	DWORD retval;

	while (1) {
		retval = WaitForMultipleObjects(3, E_hThread, TRUE, INFINITE);

		int StartTime = (int)GetTickCount64();
		while ((GetTickCount64() - StartTime) <= 10) {
			/*프레임조절 */
		}

		if (Initcheck == false)
		{
			init.timelap = timelap;
			init.gameStart = true;

			for (int i = 0; i < PATTERNCNT; ++i)
			{

				init.pattern_temp[i].x = PatternStatus[i].x;
				init.pattern_temp[i].y = PatternStatus[i].y;
				init.pattern_temp[i].x_size = PatternStatus[i].x_size;
				init.pattern_temp[i].y_size = PatternStatus[i].y_size;
				init.pattern_temp[i].objectvisible = PatternStatus[i].objectvisible;
			}
			for (int i = 0; i < BUTTONCNT; ++i)
			{
				init.button[i].objectvisible = ButtonStatus[i].objectvisible;
				init.button[i].x = ButtonStatus[i].x;
				init.button[i].y = ButtonStatus[i].y;
				init.button[i].x_size = ButtonStatus[i].x_size;
				init.button[i].y_size = ButtonStatus[i].y_size;
			}

			for (int i = 0; i < MAXCLIENT; ++i)
			{
				send(client_sock[i], (char*)&init, sizeof(init), 0);
			}
			update_packet.gamemodestate = 0;
			Initcheck = true;
		}

		UpdateFPS();
		EnterCriticalSection(&cs);
		update_packet.timelap = timelap;
		LeaveCriticalSection(&cs);

		UpdateDoor();
		UpdateFire();
		UpdatePattern();
		UpdateButton(); 

		if (timelap <= 0 && stageClear == false)
			update_packet.gamemodestate = 1;
		else if (stageClear && timelap > 0) 
			update_packet.gamemodestate = 2;


		for (int i = 0; i < MAXCLIENT; ++i)
		{

			update_packet.isJump[i] = true;

			recv(client_sock[i], (char*)&key_input, sizeof(KEYINPUT_PAKCET), MSG_WAITALL);

			playerStatus[key_input.m_id].state_type = key_input.state_type;
			playerStatus[key_input.m_id].jump = key_input.jump;
			
			UpdatePlayer(key_input.m_id);
			
			update_packet.PlayerTemp[key_input.m_id].x = playerStatus[key_input.m_id].x;
			update_packet.PlayerTemp[key_input.m_id].y = playerStatus[key_input.m_id].y;

			
			send(client_sock[i], (char*)&update_packet, sizeof(update_packet), 0);

		}

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

	InitializeCriticalSection(&cs);
	InitSettingObj();
	// 2번째 인자값 TRUE : 수동
	// 3번째 인자값 FALSE : 비신호 
	// 이벤트 신호 생성
	E_hThread[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
	E_hThread[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	E_hThread[2] = CreateEvent(NULL, TRUE, FALSE, NULL);

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
		//if (cnt == 3) 논블로킹
		//{
		//	u_long flag = 1;
		//	ioctlsocket(listen_sock, FIONBIO, &flag);
		//}

	}

	DeleteCriticalSection(&cs);
	closesocket(listen_sock);
	WSACleanup();
	CloseHandle(cli_Thread);

	return 0;
}



void UpdatePlayer(short currentId) {
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

	if (playerStatus[currentId].jump && (jumpCnt[currentId] < 2))
	{
		dropSpeed[currentId] = -6.f;
		if (jumpCnt[currentId] < 2)
			jumpCnt[currentId]++;
	}
	else
		dropSpeed[currentId] += gravity;

	if (dropSpeed[currentId] >= 0.f)
		IsCollisionFloor(currentId);

	playerStatus[currentId].x += moveSpeed[currentId];
	playerStatus[currentId].y += dropSpeed[currentId]; 
	playerStatus[currentId].CollidBox = RECT_OBJECT(playerStatus[currentId].x, playerStatus[currentId].y, 24, 28); // 콜리젼박스 업데이트
	UpdateCollisionPackage(currentId);
}

void UpdateFire() {
	for (int i = 0; i < FIRECNT; ++i) {
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
				H_FireStatus[i].x = 760; 
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

		W_FiredropSpeed[i] += W_Firegravity[i];
		H_FiredropSpeed[i] += H_Firegravity[i];

		W_FireStatus[i].x += W_FiredropSpeed[i]; 
		H_FireStatus[i].y += H_FiredropSpeed[i];

		update_packet.H_FireTemp[i].x = W_FireStatus[i].x; 
		update_packet.W_FireTemp[i].y = H_FireStatus[i].y; 

		H_FireStatus[i].CollidBox = RECT_OBJECT(H_FireStatus[i].x, H_FireStatus[i].y, H_FireStatus[i].x_size, H_FireStatus[i].y_size);
		W_FireStatus[i].CollidBox = RECT_OBJECT(W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size);
	}
}
void UpdatePattern() {

	int pc = -1;

	for (int i = 0; i < PATTERNCNT; ++i) {
		if (isPatternClear[i] == true)
			pc += 1;

		if (pc == 4) {
			update_packet.doorvisible = true; //gameClear 의미함 
			doorstatus.CollidBox = RECT_OBJECT(doorstatus.x, doorstatus.y, doorstatus.x_size, doorstatus.y_size);
		}

		if (pc == 2 && (firstbutton[0] == true || firstbutton[1] == true || firstbutton[2] == true))
		{
			PatternStatus[3].objectvisible = true;
			ButtonStatus[1].objectvisible = true;

			update_packet.PatternTemp[3].objectvisible = PatternStatus[3].objectvisible;
			update_packet.PatternTemp[3].x = PatternStatus[3].x;
			update_packet.PatternTemp[3].y = PatternStatus[3].y;
		}

		if (pc == 3 && (secondbutton[0] == true || secondbutton[1] == true || secondbutton[2] == true) &&
			(firstbutton[0] == true || firstbutton[1] == true || firstbutton[2] == true))
		{
			PatternStatus[4].objectvisible = true;
			update_packet.PatternTemp[4].objectvisible = PatternStatus[4].objectvisible;
			update_packet.PatternTemp[4].x = PatternStatus[4].x;
			update_packet.PatternTemp[4].y = PatternStatus[4].y;
		}

		if ((firstbutton[0] == false && firstbutton[1] == false && firstbutton[2] == false))
		{
			PatternStatus[3].objectvisible = false;
			update_packet.PatternTemp[3].objectvisible = PatternStatus[3].objectvisible;
			PatternStatus[4].objectvisible = false;
			update_packet.PatternTemp[4].objectvisible = PatternStatus[4].objectvisible;
			if (isPatternClear[3])
			{
				PatternStatus[3].objectvisible = true;
				update_packet.PatternTemp[3].objectvisible = PatternStatus[3].objectvisible;
			}
			if (isPatternClear[4])
			{
				PatternStatus[4].objectvisible = true;
				update_packet.PatternTemp[4].objectvisible = PatternStatus[4].objectvisible;
			}
		}

		if ((secondbutton[0] == false && secondbutton[1] == false && secondbutton[2] == false))
		{
			PatternStatus[4].objectvisible = false;
			update_packet.PatternTemp[4].objectvisible = PatternStatus[4].objectvisible;

			if (isPatternClear[4])
			{
				PatternStatus[4].objectvisible = true;
				update_packet.PatternTemp[4].objectvisible = PatternStatus[4].objectvisible;
			}
		}

		update_packet.PatternTemp[i].objectvisible = PatternStatus[i].objectvisible;
		update_packet.PatternTemp[i].x = PatternStatus[i].x;
		update_packet.PatternTemp[i].y = PatternStatus[i].y;


	}

} 
void UpdateButton() 
{
	for (int i = 0; i < BUTTONCNT; ++i)
	{
		update_packet.ButtonTemp[i].objectvisible = ButtonStatus[i].objectvisible;
	}
} 

void UpdateDoor()
{
	int ck = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (doorcheck[i]) {
			update_packet.doorcheck[i] = true;
			ck++;
		}
		else
		{
			update_packet.doorcheck[i] = false;
		}
	}

	if (ck == 3)
		stageClear = true;

} 
void IsCollisionPattern(short currentId) 
{

	for (int i = 0; i < PATTERNCNT; ++i) {
		if (PatternStatus[i].objectvisible)
		{
			if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &PatternStatus[i].CollidBox))
			{
				PatternStatus[i].x = 1200 - 30 * i; //충돌하면 우측 상단으로 이동시키는 코드
				PatternStatus[i].y = 0;
				isPatternClear[i] = true;
			}
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
			update_packet.isJump[currentId] = false;
		}
	}

}

void IsCollisionThorn(short currentId) {

	for (int i = 0; i < THORNCNT; ++i)
	{
		if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &ThornStatus[i].CollidBox))
		{
			PlayerReset(currentId); 
		}
	}

}

void IsCollisionFire(short currentId) {

	for (int i = 0; i < FIRECNT; ++i) // 불이랑 체크
	{
		if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &H_FireStatus[i].CollidBox))
		{
			PlayerReset(currentId); 
		}

		if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &W_FireStatus[i].CollidBox))
		{
			PlayerReset(currentId); 
		}
	}

}

void IsCollisionButton(short currentId) { 

	if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &ButtonStatus[0].CollidBox))
	{
		{
			firstbutton[currentId] = true;
			update_packet.firstbuttoncheck[currentId] = true;
		}
	}
	else
	{
		firstbutton[currentId] = false;
		update_packet.firstbuttoncheck[currentId] = false;
	}

	if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &ButtonStatus[1].CollidBox))
	{
		{
			secondbutton[currentId] = true;
			update_packet.secondbuttoncheck[currentId] = true;
		}
	}
	else
	{
		secondbutton[currentId] = false;
		update_packet.secondbuttoncheck[currentId] = false;
	}

} 

void IsCollisionDoor(short currentId) {

	if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &doorstatus.CollidBox))
	{
		{
			doorcheck[currentId] = true;
		}
	}
	else
		doorcheck[currentId] = false;


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
	//collisionBox생성해줌 -> 고정 
	for (int i = 0; i < FLOORCNT; ++i) {
		floorStatus[i].CollidBox = RECT_OBJECT(floorStatus[i].x, floorStatus[i].y, floorStatus[i].x_size, floorStatus[i].y_size);
	}

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
	//고정
	for (int i = 0; i < THORNCNT; ++i) {
		ThornStatus[i].CollidBox = RECT_OBJECT(ThornStatus[i].x, ThornStatus[i].y, ThornStatus[i].x_size, ThornStatus[i].y_size);
	}

	// 세로 불 초기값
	H_FireStatus[0] = Object(0, 70, -30, 34, 51);
	H_FireStatus[1] = Object(1, 380, -30, 34, 51);
	H_FireStatus[2] = Object(2, 500, -30, 34, 51);
	H_FireStatus[3] = Object(3, 760, -30, 34, 51);
	H_FireStatus[4] = Object(4, 935, -30, 34, 51);
	for (int i = 0; i < FIRECNT; ++i) {
		H_FireStatus[i].CollidBox = RECT_OBJECT(H_FireStatus[i].x, H_FireStatus[i].y, H_FireStatus[i].x_size, H_FireStatus[i].y_size);
	}

	// 가로 불 초기값
	W_FireStatus[0] = Object(0, -30, 47, 51, 34);
	W_FireStatus[1] = Object(1, -30, 218, 51, 34);
	W_FireStatus[2] = Object(2, -30, 317, 51, 34);
	W_FireStatus[3] = Object(3, -30, 482, 51, 34);
	W_FireStatus[4] = Object(4, -30, 573, 51, 34);
	for (int i = 0; i < FIRECNT; ++i) {
		W_FireStatus[i].CollidBox = RECT_OBJECT(W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size);
	}

	// 흑백 패턴 초기값 셋팅-> 콜리젼 박스 필요 없음 
	gs_PatternStatus[0] = Object(0, 1200, 0, 50, 50);
	gs_PatternStatus[1] = Object(1, 1170, 0, 50, 50);
	gs_PatternStatus[2] = Object(2, 1140, 0, 50, 50);
	gs_PatternStatus[3] = Object(3, 1110, 0, 50, 50);
	gs_PatternStatus[4] = Object(4, 1080, 0, 50, 50);

	// 문 초기값 셋팅 
	doorstatus = Object(0, 680, 305, 73, 85); 

	// 패턴의 초기 위치 셋팅
	for (int i = 0; i < PATTERNCNT; ++i)
	{
		int k = choice_uid(dre); // 랜덤값인데 안겹치게 체크 
		while (k == 2 || k == 4 || k == 6) 
		{
			k = choice_uid(dre);
		}
		while (1)
		{
			if (randomCheck[k] == false) {
				randomCheck[k] = true;
				break;
			}
			else if (randomCheck[k])
				k = choice_uid(dre); //이미 true인 곳은 다시 랜덤값 주기 
		}
		PatternStatus[i] = Object(i, floorStatus[k].x + floorStatus[k].x_size / 3, floorStatus[k].y - floorStatus[k].y_size - floorStatus[k].y_size / 2, 50, 50, true); 
		PatternStatus[i].CollidBox = RECT_OBJECT(PatternStatus[i].x, PatternStatus[i].y, PatternStatus[i].x_size, PatternStatus[i].y_size);
	}
	PatternStatus[3].objectvisible = false; 
	PatternStatus[4].objectvisible = false; 

	ButtonStatus[0] = Object(0, 100, 530, 30, 20, true);
	ButtonStatus[1] = Object(1, 600, 580, 30, 20, false); 
	for (int i = 0; i < BUTTONCNT; ++i) {
		ButtonStatus[i].CollidBox = RECT_OBJECT(ButtonStatus[i].x, ButtonStatus[i].y, ButtonStatus[i].x_size, ButtonStatus[i].y_size);
	}
}

void LoginDataSetting(int m_id)
{
	recv(client_sock[m_id], (char*)&first_login[m_id], sizeof(LOGIN_PACKET), MSG_WAITALL); 

	int k = 0;

	if (m_id == 0)
		k = 2; 
	else if (m_id == 1) 
		k = 4;
	else
		k = 6;

	login_info[m_id].player.id = m_id;
	login_info[m_id].player.state_type = PLAYER::IDLE;
	if (k == 2) 
		login_info[m_id].player.x = 380; 
	else if (k == 4) 
		login_info[m_id].player.x = 625;
	else
		login_info[m_id].player.x = 875;

	login_info[m_id].player.y = 560;
	login_info[m_id].player.visible = true;

	if (k == 2)
	{
		playerStatus[m_id].x = floorStatus[0].x;
		playerStatus[m_id].y = floorStatus[0].y;
	}
	else if (k == 4)
	{
		playerStatus[m_id].x = floorStatus[4].x;
		playerStatus[m_id].y = floorStatus[4].y;
	}
	else
	{
		playerStatus[m_id].x = floorStatus[8].x;
		playerStatus[m_id].y = floorStatus[8].y;
	}

	playerStatus[m_id].x_size = 24;
	playerStatus[m_id].y_size = 28;
	playerStatus[m_id].state_type = PLAYER::IDLE;
	playerStatus[m_id].id = m_id;

	nickname[m_id] = first_login[m_id].name;

	strcpy(login_info[m_id].name, nickname[m_id].c_str());
	strcpy(playerStatus[m_id].name, nickname[m_id].c_str());

	update_packet.PlayerTemp[m_id].id = m_id;
	update_packet.PlayerTemp[m_id].x = playerStatus[m_id].x;
	update_packet.PlayerTemp[m_id].y = playerStatus[m_id].y;
	update_packet.PlayerTemp[m_id].x_size = 24;
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

void PlayerReset(short current_Id)
{

	cout << current_Id << "번 클라이언트 충돌" << endl;
	playerStatus[current_Id].state_type = PLAYER::IDLE;
	playerStatus[current_Id].x = 640;
	playerStatus[current_Id].y = 200;
	playerStatus[current_Id].CollidBox = RECT_OBJECT(playerStatus[current_Id].x, playerStatus[current_Id].y, 24, 28);
	EnterCriticalSection(&cs); 
	timelap -= 2;
	LeaveCriticalSection(&cs); 
}
 
int m_fFPS; 
int k = 0; 

void UpdateFPS() 
{
	static DWORD FrameCnt = 0;
	static float TimeElapsed = 0;
	static DWORD lastTime = timeGetTime();

	DWORD currTime = timeGetTime();

	float timeDelta = (currTime - lastTime) * 0.001f;

	FrameCnt++;
	TimeElapsed += timeDelta;

	if (TimeElapsed >= 1.0f)
	{
		m_fFPS = (float)FrameCnt / TimeElapsed;
		TimeElapsed = 0.0f;
		FrameCnt = 0;
		k = 0;
		EnterCriticalSection(&cs);
		timelap -= 1;
		LeaveCriticalSection(&cs);
	}
	if (k == 0)
	{
		cout << "FPS : " << m_fFPS << endl;
		k++;
	}
	lastTime = currTime;
}

void UpdateCollisionPackage(short currentId) { 
	IsCollisionFire(currentId);
	IsCollisionThorn(currentId);
	IsCollisionPattern(currentId);
	IsCollisionButton(currentId);
	IsCollisionDoor(currentId);
}