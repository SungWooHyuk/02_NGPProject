#pragma warning(disable: 4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ���� 
#pragma comment(lib, "ws2_32") 
#include <winsock2.h> 
#include <iostream>
#include <random>
#include <Windows.h> 
#include "framework.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE   512  


using namespace std;

random_device rd; // �õ尪
default_random_engine dre{ rd() }; // �ʱ� ������ ���� �Ұ����ϴ�.
uniform_int_distribution<int> x_uid{ 50, 1200 }; // �� x�� ������ 
uniform_int_distribution<int> y_uid{ 30, 720 }; // �� y�� ������  
uniform_int_distribution<int> choice_uid{ 0,9 }; // ���� ��ġ ������ 

RECT rt; // ���� �簢��
RECT PlayerBox; // �÷��̾� �浹�ڽ�
RECT FloorBox[FLOORCNT]; // �ٴ� �浹�ڽ�
RECT FireBox[FIRECNT]; // �� �浹�ڽ�
RECT W_FireBox[FIRECNT]; // ���� �� �浹�ڽ�
RECT ThornBox[THORNCNT]; // ���� �浹�ڽ�
RECT PatternBox[PATTERNCNT]; // ���� �浹�ڽ� 

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Name";
LPCTSTR lpszWindowName = L"WP Game"; 
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
DWORD WINAPI Thread_client(LPVOID arg); 
HWND hWnd; // state���� ����Ϸ��� �������� ��



// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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



//������ �޾ƿͼ� �� �������ִ� �κ�  
void UpdateObject() {
	//����� �����Ұ���
}

//rendering �� �״�� Draw�� �׷��ָ� ��.
void Rendering(HDC memdc1) {
	if (isGameOver == false) // ���� �ȳ����� �̷��� �׷��ش�
	{
		BackGround.Draw(memdc1, 0, 0, 1280, 720); // ���

		//CloseDoor.Draw(memdc1, 680, 305, 73, 85); // �� ���� ũ�� 
		if (stage1Clear) {
			//stage1�� ������ ���� ���⵵�� �����  
			//�� ��ġ�� ������ ���� 


			//if (doorstatus.x_size >= 73) doorstatus.x_size = 73;
			//else doorstatus.x_size += 1;
			//if (doorstatus.y_size >= 85) doorstatus.y_size = 85;
			//else doorstatus.y_size += 1;

			//if (doorstatus.x <= 681) doorstatus.x = 680;
			//else doorstatus.x -= 0.5;
			//if (doorstatus.y <= 306) doorstatus.y = 305;
			//else doorstatus.y -= 0.95;
			//CloseDoor.Draw(memdc1, doorstatus.x, doorstatus.y, doorstatus.x_size, doorstatus.y_size); // �� ����� ���� Ŀ������ �׷��ֱ�  

		}
		else
			CloseDoor.Draw(memdc1, doorstatus.x, doorstatus.y, 1, 1); // ó������ �� �Ⱥ����ֱ�  

	
		//player�׷��ֱ�
		for (int i = 0; i < MAXCLIENT; ++i) {
			PlayerImg.Draw(memdc1, playerStatus[i].x, playerStatus[i].y, playerStatus[i].x_size, playerStatus[i].y_size); // �÷��̾� 
		}
		

		//���� �׷��ֱ�
		for (int i = 0; i < FLOORCNT; ++i)
			FloorImg.Draw(memdc1, floorStatus[i].x, floorStatus[i].y, floorStatus[i].x_size, floorStatus[i].y_size);

		//���� �׷��ֱ�
		for (int i = 0; i < THORNCNT; ++i)
			ThronImg.Draw(memdc1, ThornStatus[i].x, ThornStatus[i].y, ThornStatus[i].x_size, ThornStatus[i].y_size);
			
		//�� �׷��ֱ�
		for (int i = 0; i < FIRECNT; ++i)
			Fire.Draw(memdc1, FireStatus[i].x, FireStatus[i].y, FireStatus[i].x_size, FireStatus[i].y_size);
		
		for (int i = 0; i < FIRECNT; ++i)
			Fire2.Draw(memdc1, W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size);
			

		// ���� ������� �̸� �׷��ֱ�
		for (int i = 0; i < PATTERNCNT; ++i)
		{
			//��� ���� �׷��ֱ� 
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

		// ���� ���ӻ� �׷��ֱ� 
		
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
		0, 0, 1280, 720, // 1280 , 720 ȭ�������
		NULL, (HMENU)NULL,
		hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	//CreateThread(NULL, 0, Thread_client, NULL, 0, NULL);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

DWORD WINAPI Thread_client(LPVOID arg) {

	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");


	//�÷��̾����� ���� 
	
	while (1) {
		retval = recv(sock, (char*)&login_info, sizeof(LOGIN_PACKET), 0);  
		CurrentPlayerid = login_info.player.id;
		playerStatus[CurrentPlayerid].id = login_info.player.id;
		playerStatus[CurrentPlayerid].x = login_info.player.x;
		playerStatus[CurrentPlayerid].y = login_info.player.y; 
		//3�� �� ������ play ����
		if (currentclientcnt == 3) { 
			break;
		}
	}
	//�ʱ� �� �ޱ� ->  ���⼭ �ʱ��� ���ϰ� ��ư�� ��, �ð�, ���ӽ��� ���� 
	retval = recv(sock, (char*)&Init_info, sizeof(INIT_PACKET), 0);
	//������ �ʱ� ��ġ ����
	PatternStatus[0] = Init_info.pattern_temp[0]; 
	PatternStatus[1] = Init_info.pattern_temp[1];
	PatternStatus[2] = Init_info.pattern_temp[2];
	PatternStatus[3] = Init_info.pattern_temp[3];
	PatternStatus[4] = Init_info.pattern_temp[4]; 

	//��ư�� �ʱ� ��ġ ����
	ButtonStatus[0] = Init_info.button[0];
	ButtonStatus[1] = Init_info.button[1];

	//gamemodestate�� ó���� -1�� �״ٰ� ���⼭ 0���� �ٲ�� �����ϵ��� �ϱ� 
	//gamemodestate = Init_info.gameStart; // 0�϶��� ���� ���, 1�϶��� ���� ���� , 2�϶��� ���� Ŭ����
	//timelap = Init_info.timelap; 
									
	//update�� �ޱ� 
	while (1) {
		retval = recv(sock, (char*)&update_info, sizeof(OBJECT_UPDATE_PACKET), 0); 
		//�� ��ġ ������Ʈ
		for (int i = 0; i < FIRECNT; ++i) {
			W_FireStatus[i].x = update_info.W_FireTemp[i].x;
			W_FireStatus[i].y = update_info.W_FireTemp[i].y;
			FireStatus[i].x = update_info.H_FireTemp[i].x; 
			FireStatus[i].y = update_info.H_FireTemp[i].y;  
		}

		//���� ��ġ ������Ʈ
		for (int i = 0; i < PATTERNCNT; ++i) {
			PatternStatus[i].x = update_info.PatternTemp[i].x;
			PatternStatus[i].y = update_info.PatternTemp[i].y;
		}
		
		//��� �÷��̾� ��ġ ������Ʈ
		for (int i = 0; i < MAXCLIENT; ++i) {
			playerStatus[i].x = update_info.PlayerTemp[i].x;
			playerStatus[i].y = update_info.PlayerTemp[i].y;
		}

		//�ð�
		timelap = update_info.timelap;
		//�� ���̴��� ����
		visible = update_info.visible;
		//���Ӹ�� ���� 
		gamemodestate = update_info.gamemodestate;
	}

	closesocket(sock);

	// ���� ����
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
			InvalidateRect(hWnd, NULL, false); // ȭ�� �����ֱ�
			return 0;
			break;
		}
		break;

	case WM_CREATE: // ó�� �ʱ�ȭ ���ִ°�
		AllocConsole();
		_tfreopen(_T("CONOUT$"), _T("w"), stdout);
		_tfreopen(_T("CONIN$"), _T("r"), stdin);
		_tfreopen(_T("CONERR$"), _T("w"), stderr);

		Image(); // �̹����Լ� �ҷ��� �ʱ�ȭ
		
		InitSettingObj();
		 
		SetTimer(hWnd, 1, 10, NULL); // 1�� Ÿ�̸� , 0.01�ʸ��� �����ϰڴٴ°�
		break;

	case WM_KEYDOWN:

		if (wParam == VK_LEFT) {
			keyLayout[VK_LEFT] = 1; // ��
			keyinput_info.state_type = 2;
		}
		else if (wParam == VK_RIGHT) {
			keyLayout[VK_RIGHT] = 1; // ��
			keyinput_info.state_type = 3;
		}
		else if (wParam == VK_SPACE) { // �������������ϰ�
			if (jumpCnt  < 2){  //���� �ι����� ���� 
				isJump = true;
				jumpCnt++;
			}
			keyinput_info.state_type = 1;//����
		}
		else {
			keyinput_info.state_type = 0;//������
		}

		InvalidateRect(hWnd, NULL, false); // ȭ�� �ʱ�ȭ

		break;
	case WM_KEYUP: // Ű ���� �ҷ����°�
		if (wParam == VK_LEFT) {
			keyLayout[VK_LEFT] = 0;
		}
		else if (wParam == VK_RIGHT) {
			keyLayout[VK_RIGHT] = 0;
		}
		break;

	case WM_LBUTTONDOWN: // ���콺 ��Ŭ��
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

		DeleteObject(SelectObject(memdc1, hOldBitmap)); // ����Ʈ
		DeleteObject(hBitmap1);
		DeleteDC(memdc1);
		DeleteDC(hdc);

		EndPaint(hWnd, &ps);
	}
		break;

	case WM_DESTROY: // �ı��ɶ� �ҷ����°�
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		DeleteObject(SelectObject(memdc1, hBitmap1));
		DeleteDC(memdc1);

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

