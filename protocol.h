#pragma once
#include "Global.h"

//player���� ��Ŷ
struct LOGIN_PACKET {
	Player player[MAXCLIENT]{};
};

//object���� ��Ŷ  
struct INIT_PACKET {
	Object fire_tmp[FIRECNT]{};
	Object hfire_temp[FIRECNT]{};
	Object pattern_temp[PATTERNCNT]{};
	Object button{};
	unsigned int timelap;
	short gamestage; // ���� ��������
}; 


//player���� ��Ŷ 
struct KEYINPUT_PAKCET {
	short m_id; 
	short state_type; // IDLE, JUMP, LEFT, RIGHT 
};

//object���� ��Ŷ 
struct OBJECT_UPDATE_PACKET { 
	Object W_FireTemp[FIRECNT]{};
	Object H_FireTemp[FIRECNT]{};
	Object FloorTemp[FLOORCNT]{};
	Object PatternTemp[PATTERNCNT]{};
	Player PlayerTemp[MAXCLIENT]{};
	unsigned int timelap;
	bool visible; // �� ���̴��� �Ⱥ��̴���
	short gamemodestate; // 0 : ���� ��� , 1 : game overâ ����  , 2 : game clear â ����   
};

class Object {
public:
	Object() = default; 
	//server���� ������ ���� ������ 
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


//client�� �ʱ⿡ server�κ��� InitSettingObj()�� ���õ� �ٴ� class �迭 �޾ƿ��� ���� �Լ�  
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

//client�� server�κ��� OBJECT_UPDATE_PACKET Recv()�� ������Ʈ 
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
	//server���� ù ��ġ ������ �� �����ڷ� ���� �� ���� 
	Player(short id, short x, short y, short xs, short ys, short state)
		: Object(id, x, y, xs, ys), state_type{state} {   
		
	}

	//id���� �������� Ȯ���ϱ� ���� 
	short getId() {
		return id; 
	}

private:
	//�ʱ⿡�� id�� 0���� stat_type�� IDLE�� �⺻�� ����  
	
	short state_type{}; // ���¿� ���� type 
};

//client���� �ʱ� �ش� �÷��̾��� ���̵� ���� ���� ũ��� ��ġ �� ����
void getLoginPacket(LOGIN_PACKET& data) {  
	
}

////////////////////////////////////////////////

//server���� �ʱ⿡ OBJ�� ���� �������� �����ϴ� �Լ� 
void InitSettingObj(short stage) {
	//���⼭ �����ڷ� ������ֱ� 
	//Object{ 1,1,1,1,1,1 };
	// . . .
	//Object{ 10,10,10,10,10,10 }; �̷������� 
}



//client���� �ʱ⿡ ����, �ٴ�, ȸ������, ��� ���� �������� �����ϴ� �Լ� 
void InitSettingObj(short stage) {
	//���⼭ �����ڷ� ������ֱ� 
	
}

//Server

//�浹 
void IsCollisionFloor(short m_id);
void IsCollisionFire(short m_id);
void IsCollisionThorn(short m_id);
void IsCollisionPattern(short m_id);
void IsCollisionButton(short m_id);
void IsCollisionDoor(short m_id);

void Reset(short m_id); // �浹��C�÷��̾��� ��ġ �ʱ�ȭ isCollisionFire, isCollisionThorn �Լ����� �߻�
void CallPattern(short m_id, short patternnumber); // �浹�� ������ ��ġ�� ���� isCollisionPattern �Լ����� �߻�
void CallFloor(short m_id, short floornumber); // �ٴڰ� �浹�� dropspeed�� 0���� �ʱ�ȭ isCollsionFloor �Լ����� �߻�
void CallButton(); // ��ư�� �浹���̸� ������(5��°) ���� �����ֱ� �Լ� isCollisionButton �Լ����� �߻�
void CallDoor(); // ���� �浹������ ���� ���������� ���������� �������� ���� isCollisionDoor �Լ����� �߻�

//������Ʈ
void UpdatePlayer(KEYINPUT_PAKCET& kp); // KEYINPUT_PAKCET�� �������� packet�� �о �÷��̾� ��ġ ������Ʈ
void UpdateObject(); // ��ġ�� ����Ǵ� ������Ʈ������ ��ġ ������Ʈ

