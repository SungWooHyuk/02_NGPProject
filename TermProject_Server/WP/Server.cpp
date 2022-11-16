#pragma warning(disable: 4996)
#include <iostream>
#include <random>
#include "Server.h" 

using namespace std;

<<<<<<< Updated upstream
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
HWND hWnd; // state���� ����Ϸ��� �������� ��

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
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
=======
#define SERVERPORT 9000
#define BUFSIZE 512
#define MAXCLIENT 3

HANDLE E_hThread[3]; // �̺�Ʈ 
HANDLE Ec_hThread[3]; // �̺�Ʈ üũ
HANDLE MyUpdateThread; // �̺�Ʈ ������Ʈ

HANDLE hThread; // ������ ������ �� ����� �ڵ�
DWORD Threadid[3]; // ������ �� ����� id�� ���
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

	//�ʱ� ���� 
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

	case WM_TIMER: // �ణ Tick�Լ���� ����

		switch (wParam)
		{
		case 1: // �������� 1 ����
			State();
			InvalidateRect(hWnd, NULL, false); // ȭ�� �����ֱ�
			return 0;
			break;

		case 2:
			// �������� 2 �������� �̸� ��������.
			break;
		}

		break;

	case WM_CREATE: // ó�� �ʱ�ȭ ���ִ°�
		AllocConsole();
		_tfreopen(_T("CONOUT$"), _T("w"), stdout);
		_tfreopen(_T("CONIN$"), _T("r"), stdin);
		_tfreopen(_T("CONERR$"), _T("w"), stderr);

		Image(); // �̹����Լ� �ҷ��� �ʱ�ȭ
		doorstatus.x = 723;
		doorstatus.y = 380;
		doorstatus.x_size = 1;
		doorstatus.y_size = 1;

		SettingObjLocation();  

		SetTimer(hWnd, 1, 10, NULL); // 1�� Ÿ�̸� , 0.01�ʸ��� �����ϰڴٴ°�
		break;

	case WM_KEYDOWN:

		if (wParam == VK_LEFT) {
			keyLayout[VK_LEFT] = 1; // ��
		}
		else if (wParam == VK_RIGHT) {
			keyLayout[VK_RIGHT] = 1; // ��
		}
		else if (wParam == VK_SPACE) { // �������������ϰ�
			if (jumpCnt  < 2){  //���� �ι����� ���� 
				isJump = true;
				jumpCnt++;
			}
		}

		InvalidateRect(hWnd, NULL, false); // ȭ�� �ʱ�ȭ

		break;
	case WM_KEYUP: // Ű ���� �ҷ����°�
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

	case WM_LBUTTONDOWN: // ���콺 ��Ŭ��
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		memdc1 = CreateCompatibleDC(hdc);
		hBitmap1 = CreateCompatibleBitmap(hdc, 1280, 720);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc1, hBitmap1);

		if (isGameOver == false) // ���� �ȳ����� �̷��� �׷��ش�
		{
			BackGround.Draw(memdc1, 0, 0, 1280, 720); // ���
			 
			//CloseDoor.Draw(memdc1, 680, 305, 73, 85); // �� ���� ũ�� 
			if (stage1Clear) {
				//stage1�� ������ ���� ���⵵�� �����  
				//�� ��ġ�� ������ ���� 
				

				if (doorstatus.x_size >= 73) doorstatus.x_size = 73;
				else doorstatus.x_size += 1;
				if (doorstatus.y_size >= 85) doorstatus.y_size = 85;
				else doorstatus.y_size += 1;

				if (doorstatus.x <= 681) doorstatus.x = 680;
				else doorstatus.x -= 0.5;
				if (doorstatus.y <= 306) doorstatus.y = 305;
				else doorstatus.y -= 0.95;
				CloseDoor.Draw(memdc1, doorstatus.x, doorstatus.y, doorstatus.x_size, doorstatus.y_size); // �� ����� ���� Ŀ������ �׷��ֱ�  
				
			}
			else 
				CloseDoor.Draw(memdc1, doorstatus.x, doorstatus.y, 1,1); // ó������ �� �Ⱥ����ֱ�  

			PlayerImg.Draw(memdc1, playerStatus.x, playerStatus.y, 24, 28); // �÷��̾� 
			//RECT_DRAW�δ� �� �浹�ڽ� �����ִ°�
			//RECT_DRAW(PlayerBox);

			//���� �׷��ֱ�
			for (int i = 0; i < FLOORCNT; ++i)
			{
				FloorImg.Draw(memdc1, floorStatus[i].x, floorStatus[i].y, floorStatus[i].x_size, floorStatus[i].y_size);
				//RECT_DRAW(FloorBox[i]);
			}
			//���� �׷��ֱ�
			for (int i = 0; i < THORNCNT; ++i)
			{
				ThronImg.Draw(memdc1, ThornStatus[i].x, ThornStatus[i].y, ThornStatus[i].x_size, ThornStatus[i].y_size);
				//RECT_DRAW(ThornBox[i]);
			}
			//�� �׷��ֱ�
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
				//RECT_DRAW(PatternBox[i]);
			}
		}
		else
		{
			GameOver.Draw(memdc1, 0, 0, 1280, 720); 
		}

		BitBlt(hdc, 0, 0, 1280, 720, memdc1, 0, 0, SRCCOPY);

		DeleteObject(SelectObject(memdc1, hOldBitmap)); // ����Ʈ
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

	case WM_DESTROY: // �ı��ɶ� �ҷ����°�
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		DeleteObject(SelectObject(memdc1, hBitmap1));
		DeleteDC(memdc1);

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void SettingObjLocation() { 
	//�ٴ� ��ġ ����
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

	// ù ���� ��ġ �ٲ��� ���� 
	for (int i = 0; i < THORNCNT; ++i) 
	{
		ThornStatus[i].x = 145 * i;
		ThornBox[i] = RECT_OBJECT(ThornStatus[i].x, ThornStatus[i].y, ThornStatus[i].x_size, ThornStatus[i].y_size);
	}

	// ���� �� �ʱⰪ
	for (int i = 0; i < FIRECNT; ++i) 
	{
		FireStatus[i].x = x_uid(dre); //������ ��ġ���� ���� ���������� ����
		FireBox[i] = RECT_OBJECT(FireStatus[i].x, FireStatus[i].y, FireStatus[i].x_size, FireStatus[i].y_size);
	}

	// ���� �� �ʱⰪ
	for (int i = 0; i < FIRECNT; ++i)  
	{
		W_FireStatus[i].y = y_uid(dre);//������ ��ġ���� ���� ���η� �������� ����
		W_FireBox[i] = RECT_OBJECT(W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size);
	}

	// ��� ���� �ʱⰪ ����
	for (int i = 0; i < PATTERNCNT; ++i)
		gs_PatternStatus[i].x -= 30 * i; 

	//������ �ʱ� ��ġ ����
	for (int i = 0; i < PATTERNCNT; ++i)
	{
		int k = choice_uid(dre); // �������ε� �Ȱ�ġ�� üũ 
		while (1)
		{
			if (randomCheck[k] == false) {
				randomCheck[k] = true;
				break;
			}
			else if (randomCheck[k])
				k = choice_uid(dre); //�̹� true�� ���� �ٽ� ������ �ֱ� 
		}
		PatternStatus[i].x = floorStatus[k].x + floorStatus[k].x_size / 3; // �ڿ� + �� �߾�����
		PatternStatus[i].y = floorStatus[k].y - floorStatus[k].y_size - floorStatus[k].y_size / 2; // �ڿ� -�� ���� �÷��ְ�

		PatternBox[i] = RECT_OBJECT(PatternStatus[i].x, PatternStatus[i].y, PatternStatus[i].x_size, PatternStatus[i].y_size);
	}
} 

void State() { // ���� ���µ�
	if (isJump == true) { // ���� 2������ ����
		dropSpeed = -6.f;
		isJump = false;
	}
	else {
		dropSpeed += gravity;
	}

	if (keyLayout[VK_LEFT] == keyLayout[VK_RIGHT]) moveSpeed = 0.f; // �̵�
	else if (keyLayout[VK_LEFT]) moveSpeed = -3.f;
	else if (keyLayout[VK_RIGHT]) moveSpeed = 3.f;

	RECT PlayerBottomBox = PlayerBox; // �÷��̾� �ڽ��� �ٴ��̶� �浹�Ҷ��� �Ʒ� + ��� �浹�ڽ� ������� �ϳ� �� �����.
	PlayerBottomBox.top += 20;
	PlayerBottomBox.right -= 10;
	PlayerBottomBox.left += 10;

	if (dropSpeed >= 0.f) { // �ְ������� �浹üũ �ϰڴ�.
		for (int i = 0; i < FLOORCNT; ++i)
		{
			if (IntersectRect(&rt, &PlayerBottomBox, &FloorBox[i]))
			{
				playerStatus.y = floorStatus[i].y - 27; // �߸� �ȵ��� �� ����
				dropSpeed = 0.f;
				isJump = false;
				jumpCnt = 0;
			}
		}
	}

	for (int i = 0; i < THORNCNT; ++i) // ���ÿ� ��� �浹üũ �Ʒ� �ϴ� �ּ�.
	{
		if (IntersectRect(&rt, &PlayerBox, &ThornBox[i]))
		{
			isGameOver = true;
			//���⼭ ���� ���� ȭ�� ���� 

			//��������2���� ��� 
			//SetTimer(hWnd, 2, 10, NULL); // �̰ɷ� 2�� Ÿ�̸� �� �� ���� Timer���� switch�� 2 �� ->  
		}
	}

	playerStatus.x += moveSpeed; // ���⼭ ���� �̵�
	playerStatus.y += dropSpeed; // ���⼭ y�̵�
	
	for (int i = 0; i < FIRECNT; ++i) // ���̶� üũ
	{
		FireBox[i] = RECT_OBJECT(FireStatus[i].x, FireStatus[i].y, FireStatus[i].x_size, FireStatus[i].y_size); // ���� �� �浹�ڽ� �ʱ�ȭ   
		W_FireBox[i] = RECT_OBJECT(W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size); // ���� �� �浹�ڽ� �ʱ�ȭ    
		FiredropSpeed[i] += Firegravity[i]; // ���ǵ� �����ְ�
		FireStatus[i].y += FiredropSpeed[i]; // ���� ��ȭ
		W_FireStatus[i].x += FiredropSpeed[i]; // ���� ��ȭ

		// �Ʒ� �浹 üũ
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


		//���� ������� �ٽ� �׷��ֱ� ���� 
		if (FireStatus[i].y > 750.f) // �ʱ�ȭ
		{
			FireStatus[i].x = x_uid(dre);
			FireStatus[i].y = -30.f;
			FiredropSpeed[i] = 0.f;
		}
		if (W_FireStatus[i].x > 1300.f) //�ʱ�ȭ
		{
			W_FireStatus[i].x = -30.f;
			W_FireStatus[i].y = y_uid(dre);
		}
	}

	for (int i = 0; i < PATTERNCNT; ++i) // �����̶� �浹
	{
		int check = 0; 
		if (IntersectRect(&rt, &PlayerBox, &PatternBox[i]))
		{
			PatternStatus[i].x = 1200 - 30 * i; //�浹�ϸ� ���� ������� �̵���Ű�� �ڵ�
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

	PlayerBox = RECT_OBJECT(playerStatus.x, playerStatus.y, 24, 28); // �÷��̾�
} 