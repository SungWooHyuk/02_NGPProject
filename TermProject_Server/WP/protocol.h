#pragma once
#include "Global.h"
#include "Functions.h"
#include "Server.h"

#pragma pack (push, 1)
struct LOGIN_PACKET {
	Player player{};
	short cli_id;
};

struct INIT_PACKET {
	Object pattern_temp[PATTERNCNT]{}; // pattern object
	Object button[BUTTONCNT]{}; // button object 
	unsigned int timelap; // 초기시간
	bool gameStart; // 게임시작여부

};

struct KEYINPUT_PAKCET {
	short m_id; // 키를 입력한 플레이어의 id값
	PLAYER::Player_State state_type; // 플레이어의 상태 IDLE, JUMP, LEFT, RIGHT 
	bool jump = false;
};

struct OBJECT_UPDATE_PACKET {
	Object W_FireTemp[FIRECNT]{}; // w_fire object 업데이트 정보
	Object H_FireTemp[FIRECNT]{}; // h_fire object 업데이트 정보
	Object PatternTemp[PATTERNCNT]{}; // pattern object 업데이트 정보
	Player PlayerTemp[MAXCLIENT]{}; // player object 업데이트 정보
	unsigned int timelap; // 시간 업데이트 정보
	bool visible; // 문 보이는지 안보이는지
	short gamemodestate; //0: 기존 배경,1:gameover창 띄우기,2:gameclear 창 띄우기

};

#pragma pack(pop)