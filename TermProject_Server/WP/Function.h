//#include "Functions.h"
//
//class Player : public Object {
//public:
//	Player() = default;
//	//server���� ù ��ġ ������ �� �����ڷ� ���� �� ���� 
//	Player(short id, short x, short y, short xs, short ys, short state)
//		: Object(id, x, y, xs, ys), state_type{ state } {
//
//	}
//
//	//id���� �������� Ȯ���ϱ� ���� 
//	short getId() {
//		return id;
//	}
//
//private:
//	//�ʱ⿡�� id�� 0���� stat_type�� IDLE�� �⺻�� ����  
//
//	short state_type{}; // ���¿� ���� type 
//};
//
//
//////////////////////////////////////////////////
//
////server���� �ʱ⿡ OBJ�� ���� �������� �����ϴ� �Լ� 
//void InitSettingObj(short stage) {
//	//���⼭ �����ڷ� ������ֱ� 
//	//Object{ 1,1,1,1,1,1 };
//	// . . .
//	//Object{ 10,10,10,10,10,10 }; �̷������� 
//}
//
//
////[Server]
////�ð� �����ϴ� �Լ�  
//TIME_PACKET SetTimeLaps(unsigned int timelap) {
//	TIME_PACKET time;
//	time.timeLap = timelap;
//	return time;
//}
//
//
//
////�ʱ⿡ Player�� ���� �������� �޾ƿͼ� �����ϴ� �Լ� 
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
////����ؼ� �ٴ�, ��, ���Ͽ� ���� ������Ʈ �������� �޾ƿ��� �Լ�   
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
////���� ��� üũ �ϴ� �Լ� 
//GAMEMODE_PACKET getGameMode(GAMEMODE_PACKET& s) {
//	GAMEMODE_PACKET current_GameMode;
//	current_GameMode.isGameClear = s.isGameClear;
//	current_GameMode.isGameOver = s.isGameOver;
//	return current_GameMode;
//}
//
//
////�浹 
//
////�����Լ� 
//// 
////Server
//
////�浹 
//void IsCollisionFloor(short m_id);
//void IsCollisionFire(short m_id);
//void IsCollisionThorn(short m_id);
//void IsCollisionPattern(short m_id);
//void IsCollisionButton(short m_id);
//void IsCollisionDoor(short m_id);
//
//void Reset(short m_id); // �浹��C�÷��̾��� ��ġ �ʱ�ȭ isCollisionFire, isCollisionThorn �Լ����� �߻�
//void CallPattern(short m_id, short patternnumber); // �浹�� ������ ��ġ�� ���� isCollisionPattern �Լ����� �߻�
//void CallFloor(short m_id, short floornumber); // �ٴڰ� �浹�� dropspeed�� 0���� �ʱ�ȭ isCollsionFloor �Լ����� �߻�
//void CallButton(); // ��ư�� �浹���̸� ������(5��°) ���� �����ֱ� �Լ� isCollisionButton �Լ����� �߻�
//void CallDoor(); // ���� �浹������ ���� ���������� ���������� �������� ���� isCollisionDoor �Լ����� �߻�
//
////������Ʈ
//void UpdatePlayer(KEYINPUT_PAKCET& kp); // KEYINPUT_PAKCET�� �������� packet�� �о �÷��̾� ��ġ ������Ʈ
//void UpdateObject(); // ��ġ�� ����Ǵ� ������Ʈ������ ��ġ ������Ʈ