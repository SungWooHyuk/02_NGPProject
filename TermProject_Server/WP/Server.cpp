#pragma warning(disable: 4996)
#include <iostream>
#include <random>
#include "Server.h" 

using namespace std;

<<<<<<< Updated upstream
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




HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Name";
LPCTSTR lpszWindowName = L"WP Game"; 
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
HWND hWnd; // state에서 사용하려고 전역으로 뺌

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
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
=======
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
		login_info[m_id].player.x = 640 + (Client_count*20);
		login_info[m_id].player.y = 360;
		login_info[m_id].player.visible = true;

		//send(client_sock, (char*)&login_info[m_id], sizeof(LOGIN_PACKET), 0);
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
>>>>>>> Stashed changes
	}

	//초기 셋팅 
	INIT_PACKET s_p{};
	Object test = InitObjectData(s_p);   
	cout << test.obj_type << endl;  
	
	LOGIN_PACKET l_P{};
	Player testp = getPlayerInitData(l_P); 

<<<<<<< Updated upstream
	OBJECT_UPDATE_PACKET o_p{};
	test = getObjectData(o_p);
=======
	while (1) {
		WaitForSingleObject(Ec_hThread[m_id], INFINITE);
>>>>>>> Stashed changes

	KEYINPUT_PAKCET p_p{}; 
	testp = getPlayerData(p_p);   

	//collision(); 
	// yes - > reset();
	// no -> 
	//render();



	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
<<<<<<< Updated upstream
	PAINTSTRUCT ps;
	static HDC hdc, memdc1, memdc2;
	static HBITMAP hBitmap1, hBitmap2;
	static HBRUSH hBrush, oldBrush;

	switch (uMsg) {

	case WM_TIMER: // 약간 Tick함수라고 생각

		switch (wParam)
		{
		case 1: // 스테이지 1 느낌
			State();
			InvalidateRect(hWnd, NULL, false); // 화면 갈아주기
			return 0;
			break;

		case 2:
			// 스테이지 2 느낌으로 미리 만들어놨음.
			break;
		}

		break;

	case WM_CREATE: // 처음 초기화 해주는곳
		AllocConsole();
		_tfreopen(_T("CONOUT$"), _T("w"), stdout);
		_tfreopen(_T("CONIN$"), _T("r"), stdin);
		_tfreopen(_T("CONERR$"), _T("w"), stderr);

		Image(); // 이미지함수 불러서 초기화
		doorstatus.x = 723;
		doorstatus.y = 380;
		doorstatus.x_size = 1;
		doorstatus.y_size = 1;

		SettingObjLocation();  

		SetTimer(hWnd, 1, 10, NULL); // 1번 타이머 , 0.01초마다 갱신하겠다는거
		break;

	case WM_KEYDOWN:

		if (wParam == VK_LEFT) {
			keyLayout[VK_LEFT] = 1; // 좌
		}
		else if (wParam == VK_RIGHT) {
			keyLayout[VK_RIGHT] = 1; // 우
		}
		else if (wParam == VK_SPACE) { // 더블점프가능하게
			if (jumpCnt  < 2){  //점프 두번까지 가능 
				isJump = true;
				jumpCnt++;
			}
		}

		InvalidateRect(hWnd, NULL, false); // 화면 초기화

		break;
	case WM_KEYUP: // 키 떼면 불러지는곳
		if (wParam == VK_LEFT) {
			/*KEYINPUT_PAKCET kp;
			kp.m_id = client_id;
			kp.state_type = PLAYER_ST::LEFT;
			send(server, &kp, sizeof(kp), 0);*/
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

		if (isGameOver == false) // 게임 안끝나면 이렇게 그려준다
		{
			BackGround.Draw(memdc1, 0, 0, 1280, 720); // 배경
			 
			//CloseDoor.Draw(memdc1, 680, 305, 73, 85); // 문 원래 크기 
			if (stage1Clear) {
				//stage1이 끝나면 문이 생기도록 만들기  
				//문 위치와 사이즈 셋팅 
				

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
				CloseDoor.Draw(memdc1, doorstatus.x, doorstatus.y, 1,1); // 처음에는 문 안보여주기  

			PlayerImg.Draw(memdc1, playerStatus.x, playerStatus.y, 24, 28); // 플레이어 
			//RECT_DRAW로는 그 충돌박스 보여주는것
			//RECT_DRAW(PlayerBox);

			//발판 그려주기
			for (int i = 0; i < FLOORCNT; ++i)
			{
				FloorImg.Draw(memdc1, floorStatus[i].x, floorStatus[i].y, floorStatus[i].x_size, floorStatus[i].y_size);
				//RECT_DRAW(FloorBox[i]);
			}
			//가시 그려주기
			for (int i = 0; i < THORNCNT; ++i)
			{
				ThronImg.Draw(memdc1, ThornStatus[i].x, ThornStatus[i].y, ThornStatus[i].x_size, ThornStatus[i].y_size);
				//RECT_DRAW(ThornBox[i]);
			}
			//불 그려주기
			for (int i = 0; i < FIRECNT; ++i)
			{
				Fire.Draw(memdc1, FireStatus[i].x, FireStatus[i].y, FireStatus[i].x_size, FireStatus[i].y_size);
				//RECT_DRAW(FireBox[i]);
			}

			for (int i = 0; i < FIRECNT; ++i)
			{
				Fire2.Draw(memdc1, W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size);
				//RECT_DRAW(FireBox[i]);
			}

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
				//RECT_DRAW(PatternBox[i]);
			}
		}
		else
		{
			GameOver.Draw(memdc1, 0, 0, 1280, 720); 
		}

		BitBlt(hdc, 0, 0, 1280, 720, memdc1, 0, 0, SRCCOPY);

		DeleteObject(SelectObject(memdc1, hOldBitmap)); // 딜리트
		DeleteObject(hBitmap1);
		DeleteDC(memdc1);
		DeleteDC(hdc);

		EndPaint(hWnd, &ps);
	}
		break;
=======
	DWORD retval;
	while (1) {

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
		
		update_packet.gamemodestate = 0;
		update_packet.timelap -= 1;

		for (int i = 0; i < MAXCLIENT; ++i)
		{
			send(client_sock[i], (char*)&update_packet, sizeof(update_packet), 0);
		}

		//send()
		//Update();
		//isCollision();
		//send;

		
>>>>>>> Stashed changes

	case WM_DESTROY: // 파괴될때 불려지는곳
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		DeleteObject(SelectObject(memdc1, hBitmap1));
		DeleteDC(memdc1);

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void SettingObjLocation() { 
	//바닥 위치 셋팅
	for (int i = 0; i < FLOORCNT; ++i)
	{
		switch (i) {
		case 0:
			floorStatus[i].x = 120;
			floorStatus[i].y = 170;

			break;
		case 1:
			floorStatus[i].x = 100;
			floorStatus[i].y = 550;

			break;
		case 2:
			floorStatus[i].x = 370;
			floorStatus[i].y = 280;

			break;
		case 3:
			floorStatus[i].x = 360;
			floorStatus[i].y = 510;

			break;
		case 4:
			floorStatus[i].x = 640;
			floorStatus[i].y = 390;

			break;
		case 5:
			floorStatus[i].x = 600;
			floorStatus[i].y = 600;
			break;
		case 6:
			floorStatus[i].x = 820;
			floorStatus[i].y = 280;

			break;
		case 7:
			floorStatus[i].x = 850;
			floorStatus[i].y = 510;

			break;
		case 8:
			floorStatus[i].x = 1030;
			floorStatus[i].y = 140;

			break;
		case 9:
			floorStatus[i].x = 1050;
			floorStatus[i].y = 600;

			break;
		}
		FloorBox[i] = RECT_OBJECT(floorStatus[i].x, floorStatus[i].y, floorStatus[i].x_size, floorStatus[i].y_size - 20);
	}

	// 첫 가시 위치 바뀔일 없음 
	for (int i = 0; i < THORNCNT; ++i) 
	{
		ThornStatus[i].x = 145 * i;
		ThornBox[i] = RECT_OBJECT(ThornStatus[i].x, ThornStatus[i].y, ThornStatus[i].x_size, ThornStatus[i].y_size);
	}

	// 세로 불 초기값
	for (int i = 0; i < FIRECNT; ++i) 
	{
		FireStatus[i].x = x_uid(dre); //랜덤한 위치에서 불이 떨어지도록 셋팅
		FireBox[i] = RECT_OBJECT(FireStatus[i].x, FireStatus[i].y, FireStatus[i].x_size, FireStatus[i].y_size);
	}

	// 가로 불 초기값
	for (int i = 0; i < FIRECNT; ++i)  
	{
		W_FireStatus[i].y = y_uid(dre);//랜덤한 위치에서 불이 가로로 나오도록 셋팅
		W_FireBox[i] = RECT_OBJECT(W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size);
	}

	// 흑백 패턴 초기값 셋팅
	for (int i = 0; i < PATTERNCNT; ++i)
		gs_PatternStatus[i].x -= 30 * i; 

	//페턴의 초기 위치 셋팅
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
		PatternStatus[i].x = floorStatus[k].x + floorStatus[k].x_size / 3; // 뒤에 + 로 중앙으로
		PatternStatus[i].y = floorStatus[k].y - floorStatus[k].y_size - floorStatus[k].y_size / 2; // 뒤에 -로 위로 올려주고

		PatternBox[i] = RECT_OBJECT(PatternStatus[i].x, PatternStatus[i].y, PatternStatus[i].x_size, PatternStatus[i].y_size);
	}
} 

void State() { // 현재 상태들
	if (isJump == true) { // 점프 2번까지 가능
		dropSpeed = -6.f;
		isJump = false;
	}
	else {
		dropSpeed += gravity;
	}

	if (keyLayout[VK_LEFT] == keyLayout[VK_RIGHT]) moveSpeed = 0.f; // 이동
	else if (keyLayout[VK_LEFT]) moveSpeed = -3.f;
	else if (keyLayout[VK_RIGHT]) moveSpeed = 3.f;

	RECT PlayerBottomBox = PlayerBox; // 플레이어 박스를 바닥이랑 충돌할때는 아래 + 얇게 충돌박스 만드려고 하나 더 만든것.
	PlayerBottomBox.top += 20;
	PlayerBottomBox.right -= 10;
	PlayerBottomBox.left += 10;

	if (dropSpeed >= 0.f) { // 최고점부터 충돌체크 하겠다.
		for (int i = 0; i < FLOORCNT; ++i)
		{
			if (IntersectRect(&rt, &PlayerBottomBox, &FloorBox[i]))
			{
				playerStatus.y = floorStatus[i].y - 27; // 발목 안들어가게 좀 빼줌
				dropSpeed = 0.f;
				isJump = false;
				jumpCnt = 0;
			}
		}
	}

	for (int i = 0; i < THORNCNT; ++i) // 가시에 닿는 충돌체크 아래 일단 주석.
	{
		if (IntersectRect(&rt, &PlayerBox, &ThornBox[i]))
		{
			isGameOver = true;
			//여기서 게임 종료 화면 띄우기 

			//스테이지2에서 사용 
			//SetTimer(hWnd, 2, 10, NULL); // 이걸로 2번 타이머 쓸 수 있음 Timer에서 switch에 2 들어감 ->  
		}
	}

	playerStatus.x += moveSpeed; // 여기서 무브 이동
	playerStatus.y += dropSpeed; // 여기서 y이동
	
	for (int i = 0; i < FIRECNT; ++i) // 불이랑 체크
	{
		FireBox[i] = RECT_OBJECT(FireStatus[i].x, FireStatus[i].y, FireStatus[i].x_size, FireStatus[i].y_size); // 세로 불 충돌박스 초기화   
		W_FireBox[i] = RECT_OBJECT(W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size); // 가로 불 충돌박스 초기화    
		FiredropSpeed[i] += Firegravity[i]; // 스피드 더해주고
		FireStatus[i].y += FiredropSpeed[i]; // 상태 변화
		W_FireStatus[i].x += FiredropSpeed[i]; // 상태 변화

		// 아래 충돌 체크
		/*if (IntersectRect(&rt, &PlayerBox, &FireBox[i]))
		{
			isGameOver = true;
			SetTimer(hWnd, 2, 10, NULL);
		}

		if (IntersectRect(&rt, &PlayerBox, &W_FireBox[i]))
		{
			isGameOver = true;
			SetTimer(hWnd, 2, 10, NULL);
		}*/


		//불이 사라지면 다시 그려주기 위함 
		if (FireStatus[i].y > 750.f) // 초기화
		{
			FireStatus[i].x = x_uid(dre);
			FireStatus[i].y = -30.f;
			FiredropSpeed[i] = 0.f;
		}
		if (W_FireStatus[i].x > 1300.f) //초기화
		{
			W_FireStatus[i].x = -30.f;
			W_FireStatus[i].y = y_uid(dre);
		}
	}

	for (int i = 0; i < PATTERNCNT; ++i) // 패턴이랑 충돌
	{
		int check = 0; 
		if (IntersectRect(&rt, &PlayerBox, &PatternBox[i]))
		{
			PatternStatus[i].x = 1200 - 30 * i; //충돌하면 우측 상단으로 이동시키는 코드
			PatternStatus[i].y = 0;
			isPatternClear[i] = true; 
		}

		for (int i = 0; i < PATTERNCNT; ++i) {
			
			if (isPatternClear[i] == true)
				check += 1;
			if (check == 5) 
				stage1Clear = true;  
		}

	}

	PlayerBox = RECT_OBJECT(playerStatus.x, playerStatus.y, 24, 28); // 플레이어
} 