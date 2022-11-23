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
	unsigned int timelap; // �ʱ�ð�
	bool gameStart; // ���ӽ��ۿ���

};

struct KEYINPUT_PAKCET {
	short m_id; // Ű�� �Է��� �÷��̾��� id��
	PLAYER::Player_State state_type; // �÷��̾��� ���� IDLE, JUMP, LEFT, RIGHT 
	bool jump = false;
};

struct OBJECT_UPDATE_PACKET {
	Object W_FireTemp[FIRECNT]{}; // w_fire object ������Ʈ ����
	Object H_FireTemp[FIRECNT]{}; // h_fire object ������Ʈ ����
	Object PatternTemp[PATTERNCNT]{}; // pattern object ������Ʈ ����
	Player PlayerTemp[MAXCLIENT]{}; // player object ������Ʈ ����
	unsigned int timelap; // �ð� ������Ʈ ����
	bool visible; // �� ���̴��� �Ⱥ��̴���
	short gamemodestate; //0: ���� ���,1:gameoverâ ����,2:gameclear â ����

};

#pragma pack(pop)