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
bool isGameOver = false; // gameover ȭ�� �����ֱ����Ѱ� 
bool stage1Clear = false; //1���� �̼� ���� ����   
bool randomCheck[FLOORCNT]{ false, }; // ��������ġ�� �������ϰ� �ߺ� ���� 
bool isPatternClear[PATTERNCNT]{ false, }; //���� 5���� �� ȹ���ߴ��� �˷��ֱ� ���� ��

static float W_FiredropSpeed[FIRECNT]{ 0.f, }; // ���� �� �������� �� - ���� ���ΰ� ����� �������� �ϴ°� �׳� �̰� �������� �༭ �׷��Ű� ���κҵ� ���κ�ó�� �� �� ����.
static float H_FiredropSpeed[FIRECNT]{ 0.f, }; // ���� �� �������� �� - ���� ���ΰ� ����� �������� �ϴ°� �׳� �̰� �������� �༭ �׷��Ű� ���κҵ� ���κ�ó�� �� �� ����.
static float W_Firegravity[FIRECNT]{ 0.1f, 0.15f, 0.2f, 0.25f ,0.1f}; // �� �߷°� �� �ٸ��� �ַ���
static float H_Firegravity[FIRECNT]{ 0.1f, 0.15f, 0.2f, 0.25f ,0.1f}; // �� �߷°� �� �ٸ��� �ַ���
int timelap = 50; // �ð������� ���� ����
short gamemodestate = 0; // 0�϶��� ���� ���, 1�϶��� ���� ���� , 2�϶��� ���� Ŭ����
bool visible = false; // �ʱⰪ�� �� �Ⱥ��̰�, ���� �� �԰ԵǸ� true��
short currentclientcnt = 0; // ���� ������ Ŭ���̾�Ʈ ����
int CurrentPlayerid = 0; //�ش�client�� ���̵� 
RECT rt;	//���� �簢��

static float moveSpeed[3] = { 1.f,1.f,1.f }; // �� �� ������ ���ǵ�
static bool isJump[3] = { true,true,true }; // ���� üũ
static int jumpCnt[3] = { 0, }; // ���� ���� üũ 
static float gravity = 0.25f; // �߷°�
static float dropSpeed[3] = {0.f,0.f,0.f}; // �������� ��

static int check = 0; //���� 5���� ��� �Ծ����� Ȯ���ϴ� �뵵 
