#pragma once
//�ʱ� �����ϴ� ������ ��ũ�� 
#define RECT_DRAW(rt) Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom) //�ٿ�� �ڽ� �׸��� �Լ� 
#define RECT_MAKE(x, y, s) { x , y , x + s , y + s } // ĳ���� �ڽ�

#define RECT_OBJECT(x, y, s, s2) { x , y , x + s , y + s2 } // ������Ʈ �ڽ�
#define FLOORCNT 10 // ���� ����
#define FIRECNT 5 // �� ����
#define THORNCNT 9 // ���� ����
#define PATTERNCNT 5 // ���ϸ�� ����
#define BUTTONCNT 2 // ��ư ����
#define MAXCLIENT 3 //�÷��̾� ��  

bool keyLayout[256]{};
unsigned int timelap{}; // �ð������� ���� ����
bool gamestart = false;
bool doorvisible = false; // �ʱⰪ�� �� �Ⱥ��̰�, ���� �� �԰ԵǸ� true��
int CurrentPlayerid = 0; //�ش�client�� ���̵� 

short my_id; // �� ���̵�
bool GameStart = false;
bool doorcheck[3] = { false,false,false };
bool isJumpCheck[3] = { false,false,false };
bool firstbuttoncheck[3] = { false,false,false };
bool secondbuttoncheck[3] = { false,false,false };
short gamemodestate = 3; // 0�϶��� ���� ���, 1�϶��� ���� ���� , 2�϶��� ���� Ŭ����, 3�϶� �κ�
int tick_3 = 0;

