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
#define MAXCLIENT 3//�÷��̾� ��  

bool keyLayout[256]{}; // Ű���� ���̾ƿ��ε� �̰� ��� ����Ǳ���,,,, ���� �ڵ带 �ʹ� ��®��
bool isJump = false; // ���� üũ
bool isGameOver = false; // gameover ȭ�� �����ֱ����Ѱ� 
bool stage1Clear = false; //1���� �̼� ���� ����   
bool randomCheck[FLOORCNT]{ false, }; // ��������ġ�� �������ϰ� �ߺ� ���� 
bool isPatternClear[PATTERNCNT]{ false, }; //���� 5���� �� ȹ���ߴ��� �˷��ֱ� ���� ��

static int jumpCnt = 0; // ���� ���� üũ 
static float gravity = 0.25f; // �߷°�
static float dropSpeed = 0.f; // �������� ��
static float FiredropSpeed[FIRECNT]{ 0.f }; // ���� �� �������� �� - ���� ���ΰ� ����� �������� �ϴ°� �׳� �̰� �������� �༭ �׷��Ű� ���κҵ� ���κ�ó�� �� �� ����.
static float Firegravity[FIRECNT]{ 0.1f, 0.15f, 0.2f, 0.25f }; // �� �߷°� �� �ٸ��� �ַ���
static float moveSpeed = 1.f; // �� �� ������ ���ǵ�
unsigned int timelap{}; // �ð������� ���� ����
short gamemodestate = 0; // 0�϶��� ���� ���, 1�϶��� ���� ���� , 2�϶��� ���� Ŭ����
bool visible = false; // �ʱⰪ�� �� �Ⱥ��̰�, ���� �� �԰ԵǸ� true��
short currentclientcnt = 0; // ���� ������ Ŭ���̾�Ʈ ����
int CurrentPlayerid = 0; //�ش�client�� ���̵� 
RECT rt;   //���� �簢��