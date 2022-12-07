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
#define MAXCLIENT 3 //플레이어 수  

bool keyLayout[256]{};
unsigned int timelap{}; // 시간측정을 위한 변수
bool gamestart = false;
bool doorvisible = false; // 초기값은 문 안보이게, 문양 다 먹게되면 true로
int CurrentPlayerid = 0; //해당client의 아이디값 

short my_id; // 내 아이디
bool GameStart = false;
bool doorcheck[3] = { false,false,false };
bool isJumpCheck[3] = { false,false,false };
bool firstbuttoncheck[3] = { false,false,false };
bool secondbuttoncheck[3] = { false,false,false };
short gamemodestate = 3; // 0일때는 기존 배경, 1일때는 게임 오버 , 2일때는 게임 클리어, 3일땐 로비
int tick_3 = 0;

