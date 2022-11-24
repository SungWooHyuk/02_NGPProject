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

    Client_count++;

    client_sock[m_id] = (SOCKET)arg;

    if (logincheck[m_id] == false)
    {
        LoginDataSetting(m_id);
    }

    LoginSendPacket(Client_count);

    while (1) {

        WaitForSingleObject(Ec_hThread[m_id], INFINITE);

        //수행하고

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
        for (int i = 0; i < MAXCLIENT; ++i) {
            //여기에선 clientThread에서 키값 입력 받고 플레이어3명의 위치 모두 바꿔주었기 때문에 충돌 여부 확인 가능
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

    // 이벤트 제거
    CloseHandle(hThread);
    CloseHandle(MyUpdateThread);

    return 0;
}



void IsCollisionFloor(short currentId) {
    for (int i = 0; i < FLOORCNT; ++i)
    {
        if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &floorStatus[i].CollidBox))
        {
            playerStatus[currentId].y = floorStatus[i].y - 27; // 발목 안들어가게 좀 빼줌
            dropSpeed = 0.f;
        }
    }
}

void IsCollisionThorn(short currentId) {
    for (int i = 0; i < THORNCNT; ++i) // 가시에 닿는 충돌체크 아래 일단 주석.
    {
        if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &ThornStatus[i].CollidBox))
        {
            update_packet.gamemodestate = 1; //gameOver 의미함 
            update_packet.PlayerTemp[currentId].state_type = PLAYER::Player_State::DEAD;
        }
    }
}

void IsCollisionFire(short currentId) {
    for (int i = 0; i < FIRECNT; ++i) // 불이랑 체크
    {
        if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &FireStatus[i].CollidBox))
        {
            update_packet.gamemodestate = 1; //gameOver 의미함 
            update_packet.PlayerTemp[currentId].state_type = PLAYER::Player_State::DEAD;
        }

        if (IntersectRect(&rt, &playerStatus[currentId].CollidBox, &W_FireStatus[i].CollidBox))
        {
            update_packet.gamemodestate = 1; //gameOver 의미함 
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
    FireStatus[0] = Object(0, 70, -30, 34, 51);
    FireStatus[1] = Object(1, 380, -30, 34, 51);
    FireStatus[2] = Object(2, 500, -30, 34, 51);
    FireStatus[3] = Object(3, 710, -30, 34, 51);
    FireStatus[4] = Object(4, 935, -30, 34, 51);
    for (int i = 0; i < FIRECNT; ++i) {
        FireStatus[i].CollidBox = RECT_OBJECT(FireStatus[i].x, FireStatus[i].y, FireStatus[i].x_size, FireStatus[i].y_size);
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
    doorstatus = Object(0, 723, 380, 1, 1); //이건 문양 다 먹으면 서버쪽에서 값 계산해서 클라로 보내주면 됨
    doorstatus.CollidBox = RECT_OBJECT(doorstatus.x, doorstatus.y, doorstatus.x_size, doorstatus.y_size);


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
        //충돌 영역 위함 
        PatternStatus[i].CollidBox = RECT_OBJECT(PatternStatus[i].x, PatternStatus[i].y, PatternStatus[i].x_size, PatternStatus[i].y_size);
    }


    //버튼 초기 위치 셋팅 - 이건 정확한 값 아니고 노가다로 한번 위치 맞춰봐야함 
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