#pragma once
#include "protocol.h" 

//[Client]
// 
  

//�ʱ⿡ OBJ�� ���� �������� �޾ƿͼ� �����ϴ� �Լ� 
Object InitObjectData(INIT_PACKET& o_Data) { 
	Object o;
	o.obj_type = o_Data.type; 
	o.x_size = o_Data.imageSize_x;
	o.y_size = o_Data.imageSize_y;
	o.x = o_Data.x;
	o.y = o_Data.y; 
	return o;
}

//�ʱ⿡ Player�� ���� �������� �޾ƿͼ� �����ϴ� �Լ� 
Player getPlayerInitData(LOGIN_PACKET& p_Data) {
	Player p;
	p.m_id = p_Data.m_id;
	p.x = p_Data.x;
	p.y = p_Data.y;
	p.x_size = p_Data.x_size;
	p.y_size = p_Data.y_size; 
	return p;
}

//����ؼ� �ٴ�, ��, ���Ͽ� ���� ������Ʈ �������� �޾ƿ��� �Լ�   
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
 
//���� ��� üũ �ϴ� �Լ� 
GAMEMODE_PACKET getGameMode(GAMEMODE_PACKET& s) {
	GAMEMODE_PACKET current_GameMode;  
	current_GameMode.isGameClear = s.isGameClear;
	current_GameMode.isGameOver = s.isGameOver;
	return current_GameMode; 
}


//[Server]
//�ð� �����ϴ� �Լ�  
TIME_PACKET SetTimeLaps(unsigned int timelap) {
	TIME_PACKET time;
	time.timeLap = timelap;
	return time; 
}



//�ʱ⿡ Player�� ���� �������� �޾ƿͼ� �����ϴ� �Լ� 
Player getPlayerInitData(LOGIN_PACKET& p_Data) {
	Player p;
	p.m_id = p_Data.m_id;
	p.x = p_Data.x;
	p.y = p_Data.y;
	p.x_size = p_Data.x_size;
	p.y_size = p_Data.y_size;
	return p;
}

//����ؼ� �ٴ�, ��, ���Ͽ� ���� ������Ʈ �������� �޾ƿ��� �Լ�   
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

//���� ��� üũ �ϴ� �Լ� 
GAMEMODE_PACKET getGameMode(GAMEMODE_PACKET& s) {
	GAMEMODE_PACKET current_GameMode;
	current_GameMode.isGameClear = s.isGameClear;
	current_GameMode.isGameOver = s.isGameOver;
	return current_GameMode;
}


//�浹 

//�����Լ� 

