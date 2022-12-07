#pragma pack (push, 1)

struct LOGIN_PACKET {
	Player player{};
	short cli_id;
	char	name[20];
};

struct INIT_PACKET {
	Object pattern_temp[PATTERNCNT]{}; // pattern object
	Object button[BUTTONCNT]{}; // button object 
	unsigned int timelap; // �ʱ�ð�
	bool gameStart; // ���ӽ��ۿ���
};

struct KEYINPUT_PAKCET {
	short m_id; // Ű�� �Է��� �÷��̾��� id��
	PLAYER::Player_State  state_type; // �÷��̾��� ���� IDLE, JUMP, LEFT, RIGHT 
	bool jump = false;
};

struct OBJECT_UPDATE_PACKET {
	Object W_FireTemp[FIRECNT]{}; // w_fire object ������Ʈ ����
	Object H_FireTemp[FIRECNT]{}; // h_fire object ������Ʈ ����
	Object PatternTemp[PATTERNCNT]{}; // pattern object ������Ʈ ����
	Player PlayerTemp[MAXCLIENT]{}; // player object ������Ʈ ����
	Object ButtonTemp[BUTTONCNT]{};
	unsigned int timelap; // �ð� ������Ʈ ����
	bool doorvisible; // �� ���̴��� �Ⱥ��̴���
	short gamemodestate; //0: ���� ���,1:gameoverâ ����,2:gameclear â ����, 3: Lobby
	bool isJump[3];
	bool doorcheck[3];
	bool firstbuttoncheck[3];
	bool secondbuttoncheck[3];
};

#pragma pack(pop)


