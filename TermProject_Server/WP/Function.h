//#include "Functions.h"
//
//class Player : public Object {
//public:
//	Player() = default;
//	//server에서 첫 위치 셋팅할 때 생성자로 만들어서 쓸 것임 
//	Player(short id, short x, short y, short xs, short ys, short state)
//		: Object(id, x, y, xs, ys), state_type{ state } {
//
//	}
//
//	//id값이 무엇인지 확인하기 위함 
//	short getId() {
//		return id;
//	}
//
//private:
//	//초기에는 id는 0으로 stat_type는 IDLE로 기본값 설정  
//
//	short state_type{}; // 상태에 대한 type 
//};
//
//
//////////////////////////////////////////////////
//
////server에서 초기에 OBJ에 대한 정보값을 셋팅하는 함수 
//void InitSettingObj(short stage) {
//	//여기서 생성자로 만들어주기 
//	//Object{ 1,1,1,1,1,1 };
//	// . . .
//	//Object{ 10,10,10,10,10,10 }; 이런식으로 
//}
//
//
////[Server]
////시간 셋팅하는 함수  
//TIME_PACKET SetTimeLaps(unsigned int timelap) {
//	TIME_PACKET time;
//	time.timeLap = timelap;
//	return time;
//}
//
//
//
////초기에 Player에 대한 정보값을 받아와서 셋팅하는 함수 
//Player getPlayerInitData(LOGIN_PACKET& p_Data) {
//	Player p;
//	p_Data.player.id;
//	p.id = p_Data.player.id;
//	p.x = p_Data.x;
//	p.y = p_Data.y;
//	p.x_size = p_Data.x_size;
//	p.y_size = p_Data.y_size;
//	return p;
//}
//
////계속해서 바닥, 불, 패턴에 대한 업데이트 정보값을 받아오는 함수   
//Object getObjectData(OBJECT_UPDATE_PACKET& s_Data) {
//	Object test;
//	test.obj_type = s_Data.obj_type;
//	test.x = s_Data.x;
//	test.y = s_Data.y;
//	return test;
//}
//
////key input 
//Player getPlayerData(KEYINPUT_PAKCET& p_Data) {
//	Player p;
//	p.state_type = p_Data.state_type;
//	p.x = p_Data.x;
//	p.y = p_Data.y;
//	return p;
//}
//
////게임 모드 체크 하는 함수 
//GAMEMODE_PACKET getGameMode(GAMEMODE_PACKET& s) {
//	GAMEMODE_PACKET current_GameMode;
//	current_GameMode.isGameClear = s.isGameClear;
//	current_GameMode.isGameOver = s.isGameOver;
//	return current_GameMode;
//}
//
//
////충돌 
//
////리셋함수 
//// 
////Server
//
////충돌 
//void IsCollisionFloor(short m_id);
//void IsCollisionFire(short m_id);
//void IsCollisionThorn(short m_id);
//void IsCollisionPattern(short m_id);
//void IsCollisionButton(short m_id);
//void IsCollisionDoor(short m_id);
//
//void Reset(short m_id); // 충돌시C플레이어의 위치 초기화 isCollisionFire, isCollisionThorn 함수에서 발생
//void CallPattern(short m_id, short patternnumber); // 충돌한 패턴의 위치를 변경 isCollisionPattern 함수에서 발생
//void CallFloor(short m_id, short floornumber); // 바닥과 충돌시 dropspeed를 0으로 초기화 isCollsionFloor 함수에서 발생
//void CallButton(); // 버튼과 충돌중이면 마지막(5번째) 패턴 보여주기 함수 isCollisionButton 함수에서 발생
//void CallDoor(); // 문과 충돌했을때 다음 스테이지가 남아있으면 스테이지 변경 isCollisionDoor 함수에서 발생
//
////업데이트
//void UpdatePlayer(KEYINPUT_PAKCET& kp); // KEYINPUT_PAKCET이 들어왔으면 packet을 읽어서 플레이어 위치 없데이트
//void UpdateObject(); // 위치가 변경되는 오브젝트에대한 위치 업데이트