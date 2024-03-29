#pragma once
//초기 생성하는 변수와 매크로 
#define RECT_DRAW(rt) Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom) //바운딩 박스 그리는 함수 
#define RECT_MAKE(x, y, s) { x , y , x + s , y + s } // 캐릭터 박스
#define RECT_OBJECT(x, y, s, s2) { x , y , x + s , y + s2 } // 오브젝트 박스

#define FLOORCNT 10 // 발판 개수
#define FIRECNT 5 // 불 개수
#define THORNCNT 9 // 가시 개수
#define PATTERNCNT 5 // 패턴모양 개수
#define BUTTONCNT 2 // 버튼 개수
#define MAXCLIENT 3//플레이어 수  

bool stageClear = false; // 게임 성공 여부   
bool randomCheck[FLOORCNT]{ false, }; // 동일한위치에 없도록하게 중복 없이 
bool isPatternClear[PATTERNCNT]{ false, }; //패턴 5개를 다 획득했는지 알려주기 위한 것
static float W_FiredropSpeed[FIRECNT]{ 0.f, }; // 가로 불 떨어지는 값
static float H_FiredropSpeed[FIRECNT]{ 0.f, }; // 세로 불 떨어지는 값
static float W_Firegravity[FIRECNT]{ 0.05f,0.07f, 0.09f, 0.11f, 0.13f}; // 불 중력값 다 다르게 주려고
static float H_Firegravity[FIRECNT]{ 0.13f,0.11f, 0.09f, 0.07f, 0.05f}; // 불 중력값 다 다르게 주려고

static int timelap = 150; // 시간측정을 위한 변수
short gamemodestate = 0; // 0일때는 기존 배경, 1일때는 게임 오버 , 2일때는 게임 클리어, 3일때는 Lobby
bool visible = false; // 초기값은 문 안보이게, 문양 다 먹게되면 true로

RECT rt;	//기준 사각형

static float moveSpeed[3] = { 1.f,1.f,1.f }; // 좌 우 움직임 스피드
static int jumpCnt[3] = { 0, }; // 더블 점프 체크 
static float gravity = 0.25f; // 중력값
static float dropSpeed[3] = {0.f,0.f,0.f}; // 떨어지는 값

int cnt{};
int Client_count = 0;
bool logincheck[3]{ false, false, false };
bool Initcheck = false;

