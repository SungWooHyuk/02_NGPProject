#pragma once
#include "protocol.h" 

//[Client]
// 
  

//초기에 OBJ에 대한 정보값을 받아와서 셋팅하는 함수 
Object InitObjectData(INIT_PACKET& o_Data) { 
	Object o;
	o.obj_type = o_Data.type; 
	o.x_size = o_Data.imageSize_x;
	o.y_size = o_Data.imageSize_y;
	o.x = o_Data.x;
	o.y = o_Data.y; 
	return o;
}

//초기에 Player에 대한 정보값을 받아와서 셋팅하는 함수 
Player getPlayerInitData(LOGIN_PACKET& p_Data) {
	Player p;
	p.m_id = p_Data.m_id;
	p.x = p_Data.x;
	p.y = p_Data.y;
	p.x_size = p_Data.x_size;
	p.y_size = p_Data.y_size; 
	return p;
}

//계속해서 바닥, 불, 패턴에 대한 업데이트 정보값을 받아오는 함수   
Object getObjectData(OBJECT_UPDATE_PACKET& s_Data) {
	Object test;
	test.obj_type = s_Data.obj_type;
	test.x = s_Data.x;
	test.y = s_Data.y;
	return test;
}

//key input 
Player getPlayerData(KEYINPUT_PAKCET& p_Data) { 
	Player p;
	p.state_type = p_Data.state_type;
	p.x = p_Data.x; 
	p.y = p_Data.y;  
	return p; 
}
 
//게임 모드 체크 하는 함수 
GAMEMODE_PACKET getGameMode(GAMEMODE_PACKET& s) {
	GAMEMODE_PACKET current_GameMode;  
	current_GameMode.isGameClear = s.isGameClear;
	current_GameMode.isGameOver = s.isGameOver;
	return current_GameMode; 
}


//[Server]
//시간 셋팅하는 함수  
TIME_PACKET SetTimeLaps(unsigned int timelap) {
	TIME_PACKET time;
	time.timeLap = timelap;
	return time; 
}



//초기에 Player에 대한 정보값을 받아와서 셋팅하는 함수 
Player getPlayerInitData(LOGIN_PACKET& p_Data) {
	Player p;
	p.m_id = p_Data.m_id;
	p.x = p_Data.x;
	p.y = p_Data.y;
	p.x_size = p_Data.x_size;
	p.y_size = p_Data.y_size;
	return p;
}

//계속해서 바닥, 불, 패턴에 대한 업데이트 정보값을 받아오는 함수   
Object getObjectData(OBJECT_UPDATE_PACKET& s_Data) {
	Object test;
	test.obj_type = s_Data.obj_type;
	test.x = s_Data.x;
	test.y = s_Data.y;
	return test;
}

//key input 
Player getPlayerData(KEYINPUT_PAKCET& p_Data) {
	Player p;
	p.state_type = p_Data.state_type;
	p.x = p_Data.x;
	p.y = p_Data.y;
	return p;
}

//게임 모드 체크 하는 함수 
GAMEMODE_PACKET getGameMode(GAMEMODE_PACKET& s) {
	GAMEMODE_PACKET current_GameMode;
	current_GameMode.isGameClear = s.isGameClear;
	current_GameMode.isGameOver = s.isGameOver;
	return current_GameMode;
}


//충돌 

//리셋함수 

