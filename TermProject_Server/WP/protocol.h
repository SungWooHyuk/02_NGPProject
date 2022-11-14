#pragma once
#include "Global.h"

//player전용 패킷
struct LOGIN_PACKET {
	Player player[MAXCLIENT]{};
};

//object전용 패킷  
struct INIT_PACKET {
	Object fire_tmp[FIRECNT]{};
	Object hfire_temp[FIRECNT]{};
	Object pattern_temp[PATTERNCNT]{};
	Object button{};
	unsigned int timelap;
	short gamestage; // 게임 스테이지
}; 


//player전용 패킷 
struct KEYINPUT_PAKCET {
	short m_id; 
	short state_type; // IDLE, JUMP, LEFT, RIGHT 
};

//object전용 패킷 
struct OBJECT_UPDATE_PACKET { 
	Object W_FireTemp[FIRECNT]{};
	Object H_FireTemp[FIRECNT]{};
	Object FloorTemp[FLOORCNT]{};
	Object PatternTemp[PATTERNCNT]{};
	Player PlayerTemp[MAXCLIENT]{};
	unsigned int timelap;
	bool visible; // 문 보이는지 안보이는지
	short gamemodestate; // 0 : 기존 배경 , 1 : game over창 띄우기  , 2 : game clear 창 띄우기   
};

class Object {
public:
	Object() = default; 
	//server에서 셋팅을 위한 생성자 
	Object(short id, short x, short y, short xs, short ys)
		: id{id}, x{ x }, y{ y }, x_size{ xs }, y_size{ ys }{ 
	}

protected:
	short id{};
	short x{};
	short y{};
	short x_size{};
	short y_size{};
};


//client가 초기에 server로부터 InitSettingObj()로 셋팅된 바닥 class 배열 받아오기 위한 함수  
void getInit_W_FireInfo(INIT_PACKET& data) {
	
}

void getInit_H_FireInfo(INIT_PACKET& data) {  
	 
}

void getInit_pattern_Info(INIT_PACKET& data) {
	
}

void getInit_Button_Info(INIT_PACKET& data) { 
	 
}

void getInit_Time_Info(INIT_PACKET& data) {

}

//client가 server로부터 OBJECT_UPDATE_PACKET Recv()후 업데이트 
void Update_W_Fire(OBJECT_UPDATE_PACKET& data) {

}
void Update_H_Fire(OBJECT_UPDATE_PACKET& data) {

}
void Update_Floor(OBJECT_UPDATE_PACKET& data) {

}
void Update_Pattern(OBJECT_UPDATE_PACKET& data) {

}
void Update_Player(OBJECT_UPDATE_PACKET& data) { 

}
void Update_Time(OBJECT_UPDATE_PACKET& data) {

}
/////////////////////////////////////////////////////////////////////////////

class Player : public Object { 
public: 
	Player() = default; 
	//server에서 첫 위치 셋팅할 때 생성자로 만들어서 쓸 것임 
	Player(short id, short x, short y, short xs, short ys, short state)
		: Object(id, x, y, xs, ys), state_type{state} {   
		
	}

	//id값이 무엇인지 확인하기 위함 
	short getId() {
		return id; 
	}

private:
	//초기에는 id는 0으로 stat_type는 IDLE로 기본값 설정  
	
	short state_type{}; // 상태에 대한 type 
};

//client에서 초기 해당 플레이어의 아이디 값과 사진 크기와 위치 값 셋팅
void getLoginPacket(LOGIN_PACKET& data) {  
	
}

////////////////////////////////////////////////

//server에서 초기에 OBJ에 대한 정보값을 셋팅하는 함수 
void InitSettingObj(short stage) {
	//여기서 생성자로 만들어주기 
	//Object{ 1,1,1,1,1,1 };
	// . . .
	//Object{ 10,10,10,10,10,10 }; 이런식으로 
}



//client에서 초기에 가시, 바닥, 회색패턴, 배경 대한 정보값을 셋팅하는 함수 
void InitSettingObj(short stage) {
	//여기서 생성자로 만들어주기 
	
}

//Server

//충돌 
void IsCollisionFloor(short m_id);
void IsCollisionFire(short m_id);
void IsCollisionThorn(short m_id);
void IsCollisionPattern(short m_id);
void IsCollisionButton(short m_id);
void IsCollisionDoor(short m_id);

void Reset(short m_id); // 충돌시C플레이어의 위치 초기화 isCollisionFire, isCollisionThorn 함수에서 발생
void CallPattern(short m_id, short patternnumber); // 충돌한 패턴의 위치를 변경 isCollisionPattern 함수에서 발생
void CallFloor(short m_id, short floornumber); // 바닥과 충돌시 dropspeed를 0으로 초기화 isCollsionFloor 함수에서 발생
void CallButton(); // 버튼과 충돌중이면 마지막(5번째) 패턴 보여주기 함수 isCollisionButton 함수에서 발생
void CallDoor(); // 문과 충돌했을때 다음 스테이지가 남아있으면 스테이지 변경 isCollisionDoor 함수에서 발생

//업데이트
void UpdatePlayer(KEYINPUT_PAKCET& kp); // KEYINPUT_PAKCET이 들어왔으면 packet을 읽어서 플레이어 위치 없데이트
void UpdateObject(); // 위치가 변경되는 오브젝트에대한 위치 업데이트

