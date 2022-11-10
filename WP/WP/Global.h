#pragma once
//초기 생성하는 변수와 매크로 
#define RECT_DRAW(rt) Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom) //바운딩 박스 그리는 함수 
#define RECT_MAKE(x, y, s) { x , y , x + s , y + s } // 캐릭터 박스


#define RECT_OBJECT(x, y, s, s2) { x , y , x + s , y + s2 } // 오브젝트 박스
#define FLOORCNT 10 // 발판 개수
#define FIRECNT 5 // 불 개수
#define THORNCNT 9 // 가시 개수
#define PATTERNCNT 5 // 패턴모양 개수
#define MAXCLIENT 2 // 플레이어 개수 


Object W_Fire[FIRECNT]{}; 
Object H_Fire[FIRECNT]{};
Object Floor[FLOORCNT]{};
Object Thorn[THORNCNT]{};
Object Door{};
Object Button{};
Object Pattern[PATTERNCNT]{};
Object GrayPattern[PATTERNCNT]{};
Player player[MAXCLIENT]{};

namespace PLAYER_ST {
	enum Player_State {
		IDLE,
		LEFT,
		RIGHT,
		JUMP,
	};
}

namespace OBJECT_ST
{
	enum Objet_State {
		W_Fire, 
		H_Fire,
		Floor,
		Door, 
		Pattern,
		Player 
	};
}

bool keyLayout[256]{}; // 키보드 레이아웃인데 이거 사실 없어도되긴해,,,, 내가 코드를 너무 못짰다
bool isJump = false; // 점프 체크
bool randomCheck[FLOORCNT]{ false, }; // 동일한위치에 없도록하게 중복 없이 
bool isPatternClear[PATTERNCNT]{ false, }; //패턴 5개를 다 획득했는지 알려주기 위한 것

static int jumpCnt = 0; // 더블 점프 체크 
static float gravity = 0.25f; // 중력값
static float dropSpeed = 0.f; // 떨어지는 값
static float FiredropSpeed[FIRECNT]{ 0.f }; // 세로 불 떨어지는 값 - 지금 가로가 멈췄다 빨라졌다 하는게 그냥 이거 공통으로 줘서 그런거고 가로불도 세로불처럼 할 수 있음.
static float Firegravity[FIRECNT]{ 0.1f, 0.15f, 0.2f, 0.25f }; // 불 중력값 다 다르게 주려고
static float moveSpeed = 1.f; // 좌 우 움직임 스피드
static unsigned int time; // 시간측정을 위한 변수
static short currentstage = 1; // 현재 스테이지 초기값은 1스테이지
short gamemodestate = 0; // 0일때는 기존 배경, 1일때는 게임 오버 , 2일때는 게임 클리어
bool visible = false;   //문이 보이도록 만들기 
