#pragma warning(disable : 4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ���� 
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

HANDLE E_hThread[3]; // �̺�Ʈ 
HANDLE Ec_hThread[3]; // �̺�Ʈ üũ
HANDLE MyUpdateThread; // �̺�Ʈ ������Ʈ

HANDLE hThread; // ������ ������ �� ����� �ڵ�
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

    Client_count++;

    client_sock[m_id] = (SOCKET)arg;

    if (logincheck[m_id] == false)
    {
        LoginDataSetting(m_id);
    }

    LoginSendPacket(Client_count);

    while (1) {

        WaitForSingleObject(Ec_hThread[m_id], INFINITE);

        //�����ϰ�

        SetEvent(E_hThread[m_id]);
        ResetEvent(Ec_hThread[m_id]);

    }
    return 0;
}

DWORD WINAPI Update_Thread(LPVOID arg)
{
    DWORD retval;
    printf("����\n");
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
        for (int i = 0; i < MAXCLIENT; ++i) {
            //���⿡�� clientThread���� Ű�� �Է� �ް� �÷��̾�3���� ��ġ ��� �ٲ��־��� ������ �浹 ���� Ȯ�� ����
            IsCollisionFloor(i);
            IsCollisionThorn(i);
            IsCollisionFire(i);

        }

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
    listen(listen_sock, SOMAXCONN); // 3������ ����

    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE cli_Thread; // �����Ҷ����� ����

    // 2��° ���ڰ� TRUE : ����
    // 3��° ���ڰ� FALSE : ���ȣ 
    // �̺�Ʈ ��ȣ ����
    E_hThread[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
    E_hThread[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
    E_hThread[2] = CreateEvent(NULL, TRUE, FALSE, NULL);
    Ec_hThread[0] = CreateEvent(NULL, TRUE, TRUE, NULL);
    Ec_hThread[1] = CreateEvent(NULL, TRUE, TRUE, NULL);
    Ec_hThread[2] = CreateEvent(NULL, TRUE, TRUE, NULL);
    MyUpdateThread = CreateEvent(NULL, TRUE, TRUE, NULL); //��ȣ�� �������  

    HANDLE update_Thread = CreateThread(NULL, 0, Update_Thread, NULL, 0, NULL); // �̸� ����

    while (1)
    {
        if (cnt < 3)
        {
            addrlen = sizeof(clientaddr);
            client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);


            cli_Thread = CreateThread(NULL, 0, Client_Thread, (LPVOID)client_sock, 0, 0);

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

    // �̺�Ʈ ����
    CloseHandle(hThread);
    CloseHandle(MyUpdateThread);

    return 0;
}



void IsCollisionFloor(short currentId) {
    for (int i = 0; i < FLOORCNT; ++i)
    {
        if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &floorStatus[i].CollidBox))
        {
            playerStatus[currentId].y = floorStatus[i].y - 27; // �߸� �ȵ��� �� ����
            dropSpeed = 0.f;
        }
    }
}

void IsCollisionThorn(short currentId) {
    for (int i = 0; i < THORNCNT; ++i) // ���ÿ� ��� �浹üũ �Ʒ� �ϴ� �ּ�.
    {
        if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &ThornStatus[i].CollidBox))
        {
            update_packet.gamemodestate = 1; //gameOver �ǹ��� 
            update_packet.PlayerTemp[currentId].state_type = PLAYER::Player_State::DEAD;
        }
    }
}

void IsCollisionFire(short currentId) {
    for (int i = 0; i < FIRECNT; ++i) // ���̶� üũ
    {
        if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &FireStatus[i].CollidBox))
        {
            update_packet.gamemodestate = 1; //gameOver �ǹ��� 
            update_packet.PlayerTemp[currentId].state_type = PLAYER::Player_State::DEAD;
        }

        if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &W_FireStatus[i].CollidBox))
        {
            update_packet.gamemodestate = 1; //gameOver �ǹ��� 
            update_packet.PlayerTemp[currentId].state_type = PLAYER::Player_State::DEAD;
        }
    }
}

void InitSettingObj() {
    random_device rd; // �õ尪
    default_random_engine dre{ rd() }; // �ʱ� ������ ���� �Ұ����ϴ�.
    uniform_int_distribution<int> choice_uid{ 0,9 }; // ���� ��ġ ������  
    // �ٴ� ��ġ ����
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
    //collisionBox�������� -> ���� 
    for (int i = 0; i < FLOORCNT; ++i) {
        floorStatus[i].CollidBox = RECT_OBJECT(floorStatus[i].x, floorStatus[i].y, floorStatus[i].x_size, floorStatus[i].y_size);
    }

    // ù ���� ��ġ �ٲ��� ���� 
    ThornStatus[0] = Object(0, 0, 630, 145, 55);
    ThornStatus[1] = Object(1, 145, 630, 145, 55);
    ThornStatus[2] = Object(2, 290, 630, 145, 55);
    ThornStatus[3] = Object(3, 435, 630, 145, 55);
    ThornStatus[4] = Object(4, 580, 630, 145, 55);
    ThornStatus[5] = Object(5, 725, 630, 145, 55);
    ThornStatus[6] = Object(6, 870, 630, 145, 55);
    ThornStatus[7] = Object(7, 1015, 630, 145, 55);
    ThornStatus[8] = Object(8, 1160, 630, 145, 55);
    //����
    for (int i = 0; i < THORNCNT; ++i) {
        ThornStatus[i].CollidBox = RECT_OBJECT(ThornStatus[i].x, ThornStatus[i].y, ThornStatus[i].x_size, ThornStatus[i].y_size);
    }

    // ���� �� �ʱⰪ
    FireStatus[0] = Object(0, 70, -30, 34, 51);
    FireStatus[1] = Object(1, 380, -30, 34, 51);
    FireStatus[2] = Object(2, 500, -30, 34, 51);
    FireStatus[3] = Object(3, 710, -30, 34, 51);
    FireStatus[4] = Object(4, 935, -30, 34, 51);
    for (int i = 0; i < FIRECNT; ++i) {
        FireStatus[i].CollidBox = RECT_OBJECT(FireStatus[i].x, FireStatus[i].y, FireStatus[i].x_size, FireStatus[i].y_size);
    }

    // ���� �� �ʱⰪ
    W_FireStatus[0] = Object(0, -30, 47, 51, 34);
    W_FireStatus[1] = Object(1, -30, 218, 51, 34);
    W_FireStatus[2] = Object(2, -30, 317, 51, 34);
    W_FireStatus[3] = Object(3, -30, 482, 51, 34);
    W_FireStatus[4] = Object(4, -30, 573, 51, 34);
    for (int i = 0; i < FIRECNT; ++i) {
        W_FireStatus[i].CollidBox = RECT_OBJECT(W_FireStatus[i].x, W_FireStatus[i].y, W_FireStatus[i].x_size, W_FireStatus[i].y_size);
    }

    // ��� ���� �ʱⰪ ����-> �ݸ��� �ڽ� �ʿ� ���� 
    gs_PatternStatus[0] = Object(0, 1200, 0, 50, 50);
    gs_PatternStatus[1] = Object(1, 1170, 0, 50, 50);
    gs_PatternStatus[2] = Object(2, 1140, 0, 50, 50);
    gs_PatternStatus[3] = Object(3, 1110, 0, 50, 50);
    gs_PatternStatus[4] = Object(4, 1080, 0, 50, 50);

    // �� �ʱⰪ ���� 
    doorstatus = Object(0, 723, 380, 1, 1); //�̰� ���� �� ������ �����ʿ��� �� ����ؼ� Ŭ��� �����ָ� ��
    doorstatus.CollidBox = RECT_OBJECT(doorstatus.x, doorstatus.y, doorstatus.x_size, doorstatus.y_size);


    // ������ �ʱ� ��ġ ����
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
        PatternStatus[i] = Object(i, floorStatus[k].x + floorStatus[k].x_size / 3, floorStatus[k].y - floorStatus[k].y_size - floorStatus[k].y_size / 2, 50, 50); // �ڿ� + �� �߾�����
        //�浹 ���� ���� 
        PatternStatus[i].CollidBox = RECT_OBJECT(PatternStatus[i].x, PatternStatus[i].y, PatternStatus[i].x_size, PatternStatus[i].y_size);
    }


    //��ư �ʱ� ��ġ ���� - �̰� ��Ȯ�� �� �ƴϰ� �밡�ٷ� �ѹ� ��ġ ��������� 
    ButtonStatus[0] = Object(0, 100, 530, 120, 39);
    ButtonStatus[1] = Object(1, 600, 580, 130, 65);
    for (int i = 0; i < BUTTONCNT; ++i) {
        ButtonStatus[i].CollidBox = RECT_OBJECT(ButtonStatus[i].x, ButtonStatus[i].y, ButtonStatus[i].x_size, ButtonStatus[i].y_size);
    }


}

void LoginDataSetting(int m_id)
{
    login_info[m_id].player.id = m_id;
    login_info[m_id].player.state_type = PLAYER::IDLE;
    login_info[m_id].player.x = 640 + (Client_count * 20);
    login_info[m_id].player.y = 360;
    login_info[m_id].player.visible = true;

    playerStatus[m_id].x = 640 + (Client_count * 20);
    playerStatus[m_id].y = 360;
    playerStatus[m_id].x_size = 24;
    playerStatus[m_id].y_size = 28;
    playerStatus[m_id].state_type = PLAYER::IDLE;
    playerStatus[m_id].id = m_id;

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