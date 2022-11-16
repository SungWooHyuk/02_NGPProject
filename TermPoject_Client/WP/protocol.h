#pragma pack (push, 1)

struct Test {
	int m_id;
};
struct LOGIN_PACKET {
	Player player{};
};

struct INIT_PACKET {
	//short m_id;
	//short type; // object �з��� ���� type
	//short imageSize_x;
	//short imageSize_y;
	//short x, y;
	Object pattern_temp[PATTERNCNT]{}; // pattern object
	Object button[BUTTONCNT]{}; // button object 
	unsigned int timelap; // �ʱ�ð�
	bool gameStart; // ���ӽ��ۿ���

};

struct KEYINPUT_PAKCET {
	short m_id; // Ű�� �Է��� �÷��̾��� id��
	short state_type; // �÷��̾��� ���� IDLE, JUMP, LEFT, RIGHT
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


