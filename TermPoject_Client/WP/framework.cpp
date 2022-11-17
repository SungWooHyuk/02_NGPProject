#pragma warning(disable: 4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지 
#pragma comment(lib, "ws2_32") 
#include <winsock2.h> 
#include <WS2tcpip.h>
#include <iostream>
#include <random>
#include <Windows.h> 
#include "framework.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE   512  

using namespace std;

random_device rd; // 시드값
default_random_engine dre{ rd() }; // 초기 조건은 추적 불가능하다.
uniform_int_distribution<int> x_uid{ 50, 1200 }; // 불 x축 랜덤값 
uniform_int_distribution<int> y_uid{ 30, 720 }; // 불 y축 랜덤값  
uniform_int_distribution<int> choice_uid{ 0,9 }; // 문양 위치 랜덤값 

RECT rt; // 기준 사각형
RECT PlayerBox; // 플레이어 충돌박스
RECT FloorBox[FLOORCNT]; // 바닥 충돌박스
RECT FireBox[FIRECNT]; // 불 충돌박스
RECT W_FireBox[FIRECNT]; // 가로 불 충돌박스
RECT ThornBox[THORNCNT]; // 가시 충돌박스
RECT PatternBox[PATTERNCNT]; // 패턴 충돌박스 

SOCKET sock;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Name";
LPCTSTR lpszWindowName = L"WP Game";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
DWORD WINAPI Thread_client(LPVOID arg);
HWND hWnd; // state에서 사용하려고 전역으로 뺌
LOGIN_PACKET login_info;
INIT_PACKET  Init_info;
// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}



//데이터 받아와서 값 변경해주는 부분  
void UpdateFire(OBJECT_UPDATE_PACKET& update_info) { 
	//불 위치 업데이트
	for (int i = 0; i < FIRECNT; ++i) {
		W_FireStatus[i].x = update_info.W_FireTemp[i].x;   
		W_FireStatus[i].y = update_info.W_FireTemp[i].y;
		FireStatus[i].x = update_info.H_FireTemp[i].x;
		FireStatus[i].y = update_info.H_FireTemp[i].y;
	}
}


void UpdatePattern(OBJECT_UPDATE_PACKET& update_info) {
	//패턴 위치 업데이트
	for (int i = 0; i < PATTERNCNT; ++i) {
		PatternStatus[i].x = update_info.PatternTemp[i].x;
		PatternStatus[i].y = update_info.PatternTemp[i].y;
	}
}

void UpdatePlayer(OBJECT_UPDATE_PACKET& update_info) {
	//모든 플레이어 위치 업데이트
	for (int i = 0; i < MAXCLIENT; ++i) {
		playerStatus[i].x = update_info.PlayerTemp[i].x;
		playerStatus[i].y = update_info.PlayerTemp[i].y;
	}
}
 
void UpdateTime(OBJECT_UPDATE_PACKET& update_info) {
	timelap = update_info.timelap; 
}


void UpdatePattern(OBJECT_UPDATE_PACKET& update_info) {
	//패턴 위치 업데이트
	for (int i = 0; i < PATTERNCNT; ++i) {
		PatternStatus[i].x = update_info.PatternTemp[i].x;
		PatternStatus[i].y = update_info.PatternTemp[i].y;
	}
}

void UpdatePlayer(OBJECT_UPDATE_PACKET& update_info) {
	//모든 플레이어 위치 업데이트
	for (int i = 0; i < MAXCLIENT; ++i) {
		playerStatus[i].x = update_info.PlayerTemp[i].x;
		playerStatus[i].y = update_info.PlayerTemp[i].y;
	}
}

void UpdateTime(OBJECT_UPDATE_PACKET& update_info) {
	timelap = update_info.timelap;
}

void InitSettingObj() {
	// -  server로 부터 받아와야 하는 객체들은 id값 모두 0으로 했음
	// 플레이어 셋팅
	playerStatus[0] = Player(0, 0, 0, 24, 28, PLAYER::IDLE, false);
	playerStatus[1] = Player(0, 0, 0, 24, 28, PLAYER::IDLE, false);
	playerStatus[2] = Player(0, 0, 0, 24, 28, PLAYER::IDLE, false);

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

	// 패턴의 초기 위치 셋팅
	PatternStatus[0] = Object(0, -10, 0, 50, 50);
	PatternStatus[1] = Object(0, -10, 0, 50, 50);
	PatternStatus[2] = Object(0, -10, 0, 50, 50);
	PatternStatus[3] = Object(0, -10, 0, 50, 50);
	PatternStatus[4] = Object(0, -10, 0, 50, 50);

	// 흑백 패턴 초기값 셋팅
	gs_PatternStatus[0] = Object(0, 1200, 0, 50, 50);
	gs_PatternStatus[1] = Object(1, 1170, 0, 50, 50);
	gs_PatternStatus[2] = Object(2, 1140, 0, 50, 50);
	gs_PatternStatus[3] = Object(3, 1110, 0, 50, 50);
	gs_PatternStatus[4] = Object(4, 1080, 0, 50, 50);

	// 문 초기값 셋팅 
	doorstatus = Object(0, 723, 380, 1, 1);


}

//rendering 은 그대로 Draw로 그려주면 됨.
void Rendering(HDC memdc1) {
	if (isGameOver == false) // 게임 안끝나면 이렇게 그려준다
	{
		BackGround.Draw(memdc1, 0, 0, 1280, 720); // 배경

		//CloseDoor.Draw(memdc1, 680, 305, 73, 85); // 문 원래 크기 
		if (stage1Clear) {
			//stage1이 끝나면 문이 생기도록 만들기  
			//문 위치와 사이즈 셋팅 


			//if (doorstatus.x_size >= 73) doorstatus.x_size = 73;
			//else doorstatus.x_size += 1;
			//if (doorstatus.y_size >= 85) doorstatus.y_size = 85;
			//else doorstatus.y_size += 1;

			//if (doorstatus.x <= 681) doorstatus.x = 680;
			//else doorstatus.x -= 0.5;
			//if (doorstatus.y <= 306) doorstatus.y = 305;
			//else doorstatus.y -= 0.95;
			//CloseDoor.Draw(memdc1, doorstatus.x, doorstatus.y, doorstatus.x_size, doorstatus.y_size); // 문 사이즈가 점점 커지도록 그려주기  

		}
		else
			CloseDoor.Draw(memdc1, doorstatus.x, doorstatus.y, 1, 1); // 처음에는 문 안보여주기  


		//player그려주기
		for (int i = 0; i < MAXCLIENT; ++i) {
			if (playerStatus[i].visible)
				PlayerImg.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size); // 플레이어 
		}


		//발판 그려주기
		for (int i = 0; i < FLOORCNT; ++i)
			FloorImg.Draw(memdc1, floorStatus[i].x, floorStatus[i].y, floorStatus[i].x_size, floorStatus[i].y_size);

		//가시 그려주기
		for (int i = 0; i < THORNCNT; ++i)
			ThronImg.Draw(memdc1, ThornStatus[i].x, ThornStatus[i].y, ThornStatus[i].x_size, ThornStatus[i].y_size);

		//불 그려주기
		for (int i = 0; i < FIRECNT; ++i)
			Fire.Draw(memdc1, FireStatus[i].x, FireStatus[i].y, FireStatus[i].x_size, FireStatus[i].y_size);

		for (int i = 0; i < FIRECNT; ++i)
			Fire2.Draw(memdc1, W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size);


		// 패턴 흑백으로 미리 그려주기
		for (int i = 0; i < PATTERNCNT; ++i)
		{
			//흑백 패턴 그려주기 
			switch (i)
			{
			case 0:
				GS_Pattern.Draw(memdc1, gs_PatternStatus[i].x, gs_PatternStatus[i].y, 50, 50);
				break;
			case 1:
				GS_Pattern2.Draw(memdc1, gs_PatternStatus[i].x, gs_PatternStatus[i].y, 50, 50);
				break;
			case 2:
				GS_Pattern3.Draw(memdc1, gs_PatternStatus[i].x, gs_PatternStatus[i].y, 50, 50);
				break;
			case 3:
				GS_Pattern4.Draw(memdc1, gs_PatternStatus[i].x, gs_PatternStatus[i].y, 50, 50);
				break;
			case 4:
				GS_Pattern5.Draw(memdc1, gs_PatternStatus[i].x, gs_PatternStatus[i].y, 50, 50);
				break;
			}
		}

		// 패턴 게임상 그려주기 

		for (int i = 0; i < PATTERNCNT; ++i)
		{
			switch (i)
			{
			case 0:
				PatternImg.Draw(memdc1, PatternStatus[i].x, PatternStatus[i].y, PatternStatus[i].x_size, PatternStatus[i].y_size);
				break;
			case 1:
				Pattern2.Draw(memdc1, PatternStatus[i].x, PatternStatus[i].y, PatternStatus[i].x_size, PatternStatus[i].y_size);
				break;
			case 2:
				Pattern3.Draw(memdc1, PatternStatus[i].x, PatternStatus[i].y, PatternStatus[i].x_size, PatternStatus[i].y_size);
				break;
			case 3:
				Pattern4.Draw(memdc1, PatternStatus[i].x, PatternStatus[i].y, PatternStatus[i].x_size, PatternStatus[i].y_size);
				break;
			case 4:
				Pattern5.Draw(memdc1, PatternStatus[i].x, PatternStatus[i].y, PatternStatus[i].x_size, PatternStatus[i].y_size);
				break;
			}
		}

		//버튼 한개로 가정하고 수정함 
		if (isButtonDown)
			ButtonUP.Draw(memdc1, ButtonStatus[0].x, ButtonStatus[0].y, ButtonStatus[0].x_size, ButtonStatus[0].y_size);
		else
			ButtonDown.Draw(memdc1, ButtonStatus[0].x, ButtonStatus[0].y, ButtonStatus[0].x_size, ButtonStatus[0].y_size);
	}
	else
	{
		GameOver.Draw(memdc1, 0, 0, 1280, 720);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow
	(lpszClass, lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		0, 0, 1280, 720, // 1280 , 720 화면사이즈
		NULL, (HMENU)NULL,
		hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	CreateThread(NULL, 0, Thread_client, NULL, 0, NULL);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

DWORD WINAPI Thread_client(LPVOID arg) {

	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");


	//플레이어정보 저장 



	while (1) {
		//
		retval = recv(sock, (char*)&login_info, sizeof(LOGIN_PACKET), MSG_WAITALL);

		CurrentPlayerid = login_info.player.id;
		playerStatus[CurrentPlayerid].id = CurrentPlayerid;
		playerStatus[CurrentPlayerid].x = login_info.player.x;
		playerStatus[CurrentPlayerid].y = login_info.player.y;
		playerStatus[CurrentPlayerid].visible = login_info.player.visible;
		//3명 다 들어오고 play 시작
		if (currentclientcnt == 2) {
			break;
		}


	}
	//초기 값 받기 ->  여기서 초기의 패턴과 버튼의 값, 시간, 게임시작 여부 
	retval = recv(sock, (char*)&Init_info, sizeof(INIT_PACKET), 0);
	//패턴의 초기 위치 셋팅
	PatternStatus[0] = Init_info.pattern_temp[0];
	PatternStatus[1] = Init_info.pattern_temp[1];
	PatternStatus[2] = Init_info.pattern_temp[2];
	PatternStatus[3] = Init_info.pattern_temp[3];
	PatternStatus[4] = Init_info.pattern_temp[4];

	//버튼의 초기 위치 셋팅
	ButtonStatus[0] = Init_info.button[0];
	ButtonStatus[1] = Init_info.button[1];
	timelap = Init_info.timelap;
	//gamemodestate를 처음에 -1로 뒀다가 여기서 0으로 바뀌면 시작하도록 하기 
	//gamemodestate = Init_info.gameStart; // 0일때는 기존 배경, 1일때는 게임 오버 , 2일때는 게임 클리어

	//update값 받기 
	while (1) {
		retval = recv(sock, (char*)&update_info, sizeof(OBJECT_UPDATE_PACKET), 0); 
		//불 위치 업데이트
		UpdateFire(update_info);

		//패턴 위치 업데이트
		UpdatePattern(update_info);

		//모든 플레이어 위치 업데이트
		UpdatePlayer(update_info);
	
		//시간
		UpdateTime(update_info);
		
		//문 보이는지 여부
		visible = update_info.visible;

		//게임모드 상태 
		gamemodestate = update_info.gamemodestate;
	}
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hdc, memdc1, memdc2;
	static HBITMAP hBitmap1, hBitmap2;
	static HBRUSH hBrush, oldBrush;

	switch (uMsg) {
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			InvalidateRect(hWnd, NULL, false); // 화면 갈아주기
			return 0;
			break;
		}
		break;

	case WM_CREATE: // 처음 초기화 해주는곳
		AllocConsole();
		_tfreopen(_T("CONOUT$"), _T("w"), stdout);
		_tfreopen(_T("CONIN$"), _T("r"), stdin);
		_tfreopen(_T("CONERR$"), _T("w"), stderr);

		Image(); // 이미지함수 불러서 초기화

		InitSettingObj();

		SetTimer(hWnd, 1, 10, NULL); // 1번 타이머 , 0.01초마다 갱신하겠다는거
		break;

	case WM_KEYDOWN:

		if (wParam == VK_LEFT) {
			keyLayout[VK_LEFT] = 1; // 좌
			keyinput_info.state_type = 2;
		}
		else if (wParam == VK_RIGHT) {
			keyLayout[VK_RIGHT] = 1; // 우
			keyinput_info.state_type = 3;
		}
		else if (wParam == VK_SPACE) { // 더블점프가능하게
			if (jumpCnt < 2) {  //점프 두번까지 가능 
				isJump = true;
				jumpCnt++;
			}
			keyinput_info.state_type = 1;//점프
		}
		else {
			keyinput_info.state_type = 0;//가만히
		}

		InvalidateRect(hWnd, NULL, false); // 화면 초기화

		break;
	case WM_KEYUP: // 키 떼면 불러지는곳
		if (wParam == VK_LEFT) {
			keyLayout[VK_LEFT] = 0;
		}
		else if (wParam == VK_RIGHT) {
			keyLayout[VK_RIGHT] = 0;
		}
		break;

	case WM_LBUTTONDOWN: // 마우스 좌클릭
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		memdc1 = CreateCompatibleDC(hdc);
		hBitmap1 = CreateCompatibleBitmap(hdc, 1280, 720);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc1, hBitmap1);

		Rendering(memdc1);

		BitBlt(hdc, 0, 0, 1280, 720, memdc1, 0, 0, SRCCOPY);

		DeleteObject(SelectObject(memdc1, hOldBitmap)); // 딜리트
		DeleteObject(hBitmap1);
		DeleteDC(memdc1);
		DeleteDC(hdc);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY: // 파괴될때 불려지는곳
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		DeleteObject(SelectObject(memdc1, hBitmap1));
		DeleteDC(memdc1);

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

