#pragma warning(disable: 4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지 
#pragma comment(lib, "ws2_32") 
#include <winsock2.h> 
#include <WS2tcpip.h>
#include <Windows.h> 
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <string> 
#include <stdlib.h> 

#include "framework.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE   512  

using namespace std;

LOGIN_PACKET first_login;
LOGIN_PACKET login_info;
INIT_PACKET  Init_info;

SOCKET sock;
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Name";
LPCTSTR lpszWindowName = L"Treasure Hunter";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
DWORD WINAPI Thread_client(LPVOID arg);
HWND hWnd;

wchar_t temp_ipBuf[256];
wchar_t temp_idBuf[256];
string playerid;
string ip;

string chracter_name[3];
LPCTSTR DC = L"Check";

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


void InitSettingObj();
void LoginDataSetting(LOGIN_PACKET& login_info);
void InitDataSetting(INIT_PACKET& init_info);
void UpdateTime(OBJECT_UPDATE_PACKET& update_info);
void UpdatePlayer(OBJECT_UPDATE_PACKET& update_info);
void UpdatePattern(OBJECT_UPDATE_PACKET& update_info);
void UpdateFire(OBJECT_UPDATE_PACKET& update_info);
void UpdateButton(OBJECT_UPDATE_PACKET& update_info);

void Rendering(HDC memdc1);

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

HANDLE hWriteEvent;
HWND hButtonDialog;
HWND hEditDialog;
HWND hEdit_2_Dialog;

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
		0, 0, 1280, 720,
		NULL, (HMENU)NULL,
		hInstance, NULL);



	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG11), NULL, DlgProc);

	ShowWindow(hWnd, nCmdShow);

	CreateThread(NULL, 0, Thread_client, NULL, 0, NULL);

	UpdateWindow(hWnd);

	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	switch (iMessage)
	{
	case WM_INITDIALOG:
		hEditDialog = GetDlgItem(hDlg, IDC_EDIT11);
		hEdit_2_Dialog = GetDlgItem(hDlg, IDC_EDIT21);
		hButtonDialog = GetDlgItem(hDlg, IDC_BUTTON11);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON11:
			GetDlgItemText(hDlg, IDC_EDIT11, temp_ipBuf, 256);
			GetDlgItemText(hDlg, IDC_EDIT21, temp_idBuf, 256);
			SetEvent(hWriteEvent);
			EndDialog(hDlg, IDD_DIALOG11);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:

		EndDialog(hDlg, IDD_DIALOG11);
		return TRUE;
	}

	return FALSE;
}



DWORD WINAPI Thread_client(LPVOID arg) {

	WaitForSingleObject(hWriteEvent, INFINITE);

	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");


	int cnt = 0;

	for (int i = 0; temp_ipBuf[i] != NULL; ++i)
	{
		ip += (char)temp_ipBuf[i];
	}

	for (int i = 0; temp_idBuf[i] != NULL; ++i)
	{
		playerid += (char)temp_idBuf[i];
	}


	//if (ip == "127.0.0.1" || ip == "192.168.207.197")
	//{
	//}
	//else
	//	exit(-1);

	strcpy(first_login.name, playerid.c_str());

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr((PCSTR)&ip);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");


	send(sock, (char*)&first_login, sizeof(LOGIN_PACKET), 0);

	while (1) {
		retval = recv(sock, (char*)&login_info, sizeof(LOGIN_PACKET), MSG_WAITALL);
		CurrentPlayerid = login_info.player.id;
		LoginDataSetting(login_info);
		//3명 다 들어오고 play 시작
		if (CurrentPlayerid == 2) {
			my_id = login_info.cli_id;
			break;
		}
	}

	retval = recv(sock, (char*)&Init_info, sizeof(INIT_PACKET), 0);
	InitDataSetting(Init_info);
	GameStart = true;

	while (1) {
		if (GameStart)
		{
			if (keyLayout[VK_LEFT] == keyLayout[VK_RIGHT]) keyinput_info.state_type = PLAYER::Player_State::IDLE; // 이동
			else if (keyLayout[VK_LEFT]) keyinput_info.state_type = PLAYER::Player_State::LEFT;
			else if (keyLayout[VK_RIGHT]) keyinput_info.state_type = PLAYER::Player_State::RIGHT;

			if (keyLayout[VK_SPACE])
			{
				keyinput_info.jump = true;
			}

			send(sock, (char*)&keyinput_info, sizeof(KEYINPUT_PAKCET), 0);

			keyLayout[VK_SPACE] = 0;
			keyinput_info.jump = false;

			retval = recv(sock, (char*)&update_info, sizeof(OBJECT_UPDATE_PACKET), 0);

			UpdateFire(update_info);
			UpdatePlayer(update_info);
			UpdatePattern(update_info);
			UpdateTime(update_info);
			UpdateButton(update_info);

			for (int i = 0; i < 3; ++i) {
				doorcheck[i] = update_info.doorcheck[i];
				isJumpCheck[i] = update_info.isJump[i];
				firstbuttoncheck[i] = update_info.firstbuttoncheck[i];
				secondbuttoncheck[i] = update_info.secondbuttoncheck[i];
			}

			doorvisible = update_info.doorvisible;
			gamemodestate = update_info.gamemodestate;

		}
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}

TCHAR* StringToTCHAR(string& s)
{
	string tstr;
	const char* all = s.c_str();
	int len = 1 + strlen(all);
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	static HDC hdc, memdc1, memdc2;
	static HBITMAP hBitmap1, hBitmap2;
	static HBRUSH hBrush, oldBrush;

	static TCHAR sTime[128] = _T("");
	static TCHAR name[128] = _T("");
	static TCHAR two_name[128] = _T("");
	static TCHAR three_name[128] = _T("");

	switch (uMsg) {
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (gamemodestate == 0 || gamemodestate == 3)
			{
				if (GameStart)
					_stprintf_s(sTime, _T("남은 시간은 %d 입니다."), timelap);

				_stprintf_s(name, L"%s", StringToTCHAR(chracter_name[0]));
				_stprintf_s(two_name, L"%s", StringToTCHAR(chracter_name[1]));
				_stprintf_s(three_name, L"%s", StringToTCHAR(chracter_name[2]));


				if (doorcheck[0] != true)
					_stprintf_s(name, L"%s", StringToTCHAR(chracter_name[0]));
				else
					_stprintf_s(name, L"%s", DC);

				if (doorcheck[1] != true)
					_stprintf_s(two_name, L"%s", StringToTCHAR(chracter_name[1]));
				else
					_stprintf_s(two_name, L"%s", DC);

				if (doorcheck[2] != true)
					_stprintf_s(three_name, L"%s", StringToTCHAR(chracter_name[2]));
				else
					_stprintf_s(three_name, L"%s", DC);

				InvalidateRect(hWnd, NULL, false);
			}
			else
			{
				InvalidateRect(hWnd, NULL, false);
				return 0;
			}
			break;
		}
		break;

	case WM_CREATE: // 처음 초기화 해주는곳
		//AllocConsole();
		///_tfreopen(_T("CONOUT$"), _T("w"), stdout);
		///_tfreopen(_T("CONIN$"), _T("r"), stdin);
		//_tfreopen(_T("CONERR$"), _T("w"), stderr);

		Image();

		InitSettingObj();

		SetTimer(hWnd, 1, 10, NULL); // 1번 타이머 , 0.01초마다 갱신하겠다는거
		break;

	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			keyLayout[VK_LEFT] = 1; // 좌
		}
		else if (wParam == VK_RIGHT) {
			keyLayout[VK_RIGHT] = 1; // 우
		}
		else if (wParam == VK_SPACE) {
			keyLayout[VK_SPACE] = 1;
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

		if (gamemodestate == 0 || gamemodestate == 3)
		{
			SetBkMode(memdc1, TRANSPARENT);
			SetTextColor(memdc1, RGB(255, 255, 255));
			TextOut(memdc1, 540, 50, sTime, _tcslen(sTime));

			SetBkMode(memdc1, TRANSPARENT);
			SetTextColor(memdc1, RGB(255, 0, 0));
			TextOut(memdc1, playerStatus[0].x - strlen(chracter_name[0].c_str()), playerStatus[0].y - 20, name, _tcslen(name));

			SetBkMode(memdc1, TRANSPARENT);
			SetTextColor(memdc1, RGB(0, 255, 0));
			TextOut(memdc1, playerStatus[1].x - strlen(chracter_name[1].c_str()), playerStatus[1].y - 20, two_name, _tcslen(two_name));

			SetBkMode(memdc1, TRANSPARENT);
			SetTextColor(memdc1, RGB(0, 0, 255));
			TextOut(memdc1, playerStatus[2].x - strlen(chracter_name[2].c_str()), playerStatus[2].y - 20, three_name, _tcslen(three_name));
		}

		BitBlt(hdc, 0, 0, 1280, 720, memdc1, 0, 0, SRCCOPY);

		DeleteObject(SelectObject(memdc1, hOldBitmap));
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

void LoginDataSetting(LOGIN_PACKET& login_info)
{
	playerStatus[login_info.player.id].id = login_info.player.id;
	playerStatus[login_info.player.id].x = login_info.player.x;
	playerStatus[login_info.player.id].y = login_info.player.y;
	playerStatus[login_info.player.id].visible = login_info.player.visible;
	chracter_name[login_info.player.id] = login_info.name;
	strcpy(playerStatus[login_info.player.id].name, chracter_name[login_info.player.id].c_str());
}

void InitDataSetting(INIT_PACKET& init_info)
{
	keyinput_info.m_id = my_id;
	keyinput_info.state_type = PLAYER::IDLE;

	for (int i = 0; i < PATTERNCNT; ++i)
	{
		PatternStatus[i].x = init_info.pattern_temp[i].x;
		PatternStatus[i].y = init_info.pattern_temp[i].y;
		PatternStatus[i].x_size = init_info.pattern_temp[i].x_size;
		PatternStatus[i].y_size = init_info.pattern_temp[i].y_size;
		PatternStatus[i].id = init_info.pattern_temp[i].id;
		PatternStatus[i].objectvisible = init_info.pattern_temp[i].objectvisible;
	}
	for (int i = 0; i < BUTTONCNT; ++i)
	{
		ButtonStatus[i].objectvisible = init_info.button[i].objectvisible;
		ButtonStatus[i].x = init_info.button[i].x;
		ButtonStatus[i].y = init_info.button[i].y;
		ButtonStatus[i].x_size = init_info.button[i].x_size;
		ButtonStatus[i].y_size = init_info.button[i].y_size;
		ButtonStatus[i].id = init_info.button[i].id;
	}

	timelap = init_info.timelap;
	gamestart = init_info.gameStart;
}

void UpdateFire(OBJECT_UPDATE_PACKET& update_info) {

	for (int i = 0; i < FIRECNT; ++i) {
		W_FireStatus[i].x = update_info.H_FireTemp[i].x;
		FireStatus[i].y = update_info.W_FireTemp[i].y;
	}
}


void UpdatePattern(OBJECT_UPDATE_PACKET& update_info) {
	for (int i = 0; i < PATTERNCNT; ++i) {
		if (update_info.PatternTemp[i].objectvisible)
		{
			PatternStatus[i].x = update_info.PatternTemp[i].x;
			PatternStatus[i].y = update_info.PatternTemp[i].y;
			PatternStatus[i].objectvisible = update_info.PatternTemp[i].objectvisible;
		}
		else
		{
			PatternStatus[i].objectvisible = update_info.PatternTemp[i].objectvisible;
		}
	}
}

void UpdateButton(OBJECT_UPDATE_PACKET& update_info)
{
	for (int i = 0; i < BUTTONCNT; ++i)
	{
		ButtonStatus[i].objectvisible = update_info.ButtonTemp[i].objectvisible;
	}
}
void UpdatePlayer(OBJECT_UPDATE_PACKET& update_info) {
	for (int i = 0; i < MAXCLIENT; ++i) {
		playerStatus[i].x = update_info.PlayerTemp[i].x;
		playerStatus[i].y = update_info.PlayerTemp[i].y;
	}
}


void UpdateTime(OBJECT_UPDATE_PACKET& update_info) {
	timelap = update_info.timelap;
}

void InitSettingObj() {
	playerStatus[0] = Player(0, 0, 0, 24, 28, PLAYER::IDLE, false);
	playerStatus[1] = Player(0, 0, 0, 24, 28, PLAYER::IDLE, false);
	playerStatus[2] = Player(0, 0, 0, 24, 28, PLAYER::IDLE, false);

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

	ThornStatus[0] = Object(0, 0, 630, 145, 55);
	ThornStatus[1] = Object(1, 145, 630, 145, 55);
	ThornStatus[2] = Object(2, 290, 630, 145, 55);
	ThornStatus[3] = Object(3, 435, 630, 145, 55);
	ThornStatus[4] = Object(4, 580, 630, 145, 55);
	ThornStatus[5] = Object(5, 725, 630, 145, 55);
	ThornStatus[6] = Object(6, 870, 630, 145, 55);
	ThornStatus[7] = Object(7, 1015, 630, 145, 55);
	ThornStatus[8] = Object(8, 1160, 630, 145, 55);

	FireStatus[0] = Object(0, 70, -50, 34, 51);
	FireStatus[1] = Object(1, 380, -50, 34, 51);
	FireStatus[2] = Object(2, 500, -50, 34, 51);
	FireStatus[3] = Object(3, 710, -50, 34, 51);
	FireStatus[4] = Object(4, 935, -50, 34, 51);

	W_FireStatus[0] = Object(0, -55, 47, 51, 34);
	W_FireStatus[1] = Object(1, -55, 218, 51, 34);
	W_FireStatus[2] = Object(2, -55, 317, 51, 34);
	W_FireStatus[3] = Object(3, -55, 482, 51, 34);
	W_FireStatus[4] = Object(4, -55, 573, 51, 34);

	PatternStatus[0] = Object(0, -50, -50, 50, 50);
	PatternStatus[1] = Object(0, -50, -50, 50, 50);
	PatternStatus[2] = Object(0, -50, -50, 50, 50);
	PatternStatus[3] = Object(0, -50, -50, 50, 50);
	PatternStatus[4] = Object(0, -50, -50, 50, 50);

	gs_PatternStatus[0] = Object(0, 1200, 0, 50, 50);
	gs_PatternStatus[1] = Object(1, 1170, 0, 50, 50);
	gs_PatternStatus[2] = Object(2, 1140, 0, 50, 50);
	gs_PatternStatus[3] = Object(3, 1110, 0, 50, 50);
	gs_PatternStatus[4] = Object(4, 1080, 0, 50, 50);

	doorstatus = Object(0, 723, 380, 1, 1);

}

void Rendering(HDC memdc1) {
	if (gamemodestate == 0)
	{
		BackGround.Draw(memdc1, 0, 0, 1280, 720);

		if (doorvisible) {
			if (doorstatus.x_size >= 73) doorstatus.x_size = 73;
			else doorstatus.x_size += 1;
			if (doorstatus.y_size >= 85) doorstatus.y_size = 85;
			else doorstatus.y_size += 1;

			if (doorstatus.x <= 681) doorstatus.x = 680;
			else doorstatus.x -= 0.5;
			if (doorstatus.y <= 306) doorstatus.y = 305;
			else doorstatus.y -= 0.95;
			CloseDoor.Draw(memdc1, doorstatus.x, doorstatus.y, doorstatus.x_size, doorstatus.y_size); // 문 사이즈가 점점 커지도록 그려주기  
		}
		else
			CloseDoor.Draw(memdc1, doorstatus.x, doorstatus.y, 1, 1); // 처음에는 문 안보여주기  

		for (int i = 0; i < MAXCLIENT; ++i) {
			if (playerStatus[i].visible && GameStart)
			{
				switch (playerStatus[i].id)
				{
				case 0:
					if (isJumpCheck[i])
						PlayerImg_j.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);
					else
						PlayerImg.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);
					break;
				case 1:
					if (isJumpCheck[i])
						PlayerImg_j_2.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);
					else
						PlayerImg_2.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);

					break;
				case 2:
					if (isJumpCheck[i])
						PlayerImg_j_3.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);
					else
						PlayerImg_3.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);
					break;

				}
			}
		}

		if (GameStart)
		{
			for (int i = 0; i < FLOORCNT; ++i)
				FloorImg.Draw(memdc1, floorStatus[i].x, floorStatus[i].y, floorStatus[i].x_size, floorStatus[i].y_size);
		}

		if (GameStart) {
			for (int i = 0; i < THORNCNT; ++i)
				ThronImg.Draw(memdc1, ThornStatus[i].x, ThornStatus[i].y, ThornStatus[i].x_size, ThornStatus[i].y_size);
		}
		for (int i = 0; i < FIRECNT; ++i)
			Fire.Draw(memdc1, FireStatus[i].x, FireStatus[i].y, FireStatus[i].x_size, FireStatus[i].y_size);

		for (int i = 0; i < FIRECNT; ++i)
			Fire2.Draw(memdc1, W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size);

		for (int i = 0; i < PATTERNCNT; ++i)
		{
			if (GameStart) {
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
		}

		for (int i = 0; i < PATTERNCNT; ++i)
		{
			if (PatternStatus[i].objectvisible)
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
		}

		for (int i = 0; i < BUTTONCNT; ++i)
		{
			if (ButtonStatus[i].objectvisible)
			{
				switch (i)
				{
				case 0:
					if (firstbuttoncheck[0] || firstbuttoncheck[1] || firstbuttoncheck[2])
						ButtonDown.Draw(memdc1, ButtonStatus[i].x, ButtonStatus[i].y + 10, ButtonStatus[i].x_size, ButtonStatus[i].y_size - 10);
					else
						ButtonUP.Draw(memdc1, ButtonStatus[i].x, ButtonStatus[i].y, ButtonStatus[i].x_size, ButtonStatus[i].y_size);
					break;

				case 1:
					if (secondbuttoncheck[0] || secondbuttoncheck[1] || secondbuttoncheck[2])
						ButtonDown.Draw(memdc1, ButtonStatus[i].x, ButtonStatus[i].y + 10, ButtonStatus[i].x_size, ButtonStatus[i].y_size - 10);
					else
						ButtonUP.Draw(memdc1, ButtonStatus[i].x, ButtonStatus[i].y, ButtonStatus[i].x_size, ButtonStatus[i].y_size);
					break;

				}
			}
		}
	}


	else if (gamemodestate == 1)
	{
		GameOver.Draw(memdc1, 0, 0, 1280, 720);
	}
	else if (gamemodestate == 2)
	{
		GameClear.Draw(memdc1, 0, 0, 1280, 720);
	}
	else if (gamemodestate == 3)
	{
		Lobby.Draw(memdc1, 0, 0, 1280, 720);

		for (int i = 0; i < MAXCLIENT; ++i) {
			if (playerStatus[i].visible && GameStart == false)
			{
				if (tick_3 < 50)
				{
					switch (playerStatus[i].id)
					{
					case 0:
						PlayerImg.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);
						break;
					case 1:
						PlayerImg_2.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);
						break;
					case 2:
						PlayerImg_3.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);
						break;
					}
				}
				else
				{
					switch (playerStatus[i].id)
					{
					case 0:
						PlayerImg_j.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);
						break;
					case 1:
						PlayerImg_j_2.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);
						break;
					case 2:
						PlayerImg_j_3.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size);
						break;
					}

					if (tick_3 > 100)
						tick_3 = 0;
				}
			}

		}
		tick_3++;
	}
}