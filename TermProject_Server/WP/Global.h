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

bool stageClear = false; // ���� ���� ����   
bool randomCheck[FLOORCNT]{ false, }; // ��������ġ�� �������ϰ� �ߺ� ���� 
bool isPatternClear[PATTERNCNT]{ false, }; //���� 5���� �� ȹ���ߴ��� �˷��ֱ� ���� ��
static float W_FiredropSpeed[FIRECNT]{ 0.f, }; // ���� �� �������� ��
static float H_FiredropSpeed[FIRECNT]{ 0.f, }; // ���� �� �������� ��
static float W_Firegravity[FIRECNT]{ 0.05f,0.07f, 0.09f, 0.11f, 0.13f}; // �� �߷°� �� �ٸ��� �ַ���
static float H_Firegravity[FIRECNT]{ 0.13f,0.11f, 0.09f, 0.07f, 0.05f}; // �� �߷°� �� �ٸ��� �ַ���

static int timelap = 150; // �ð������� ���� ����
short gamemodestate = 0; // 0�϶��� ���� ���, 1�϶��� ���� ���� , 2�϶��� ���� Ŭ����, 3�϶��� Lobby
bool visible = false; // �ʱⰪ�� �� �Ⱥ��̰�, ���� �� �԰ԵǸ� true��

RECT rt;	//���� �簢��

static float moveSpeed[3] = { 1.f,1.f,1.f }; // �� �� ������ ���ǵ�
static int jumpCnt[3] = { 0, }; // ���� ���� üũ 
static float gravity = 0.25f; // �߷°�
static float dropSpeed[3] = {0.f,0.f,0.f}; // �������� ��

int cnt{};
int Client_count = 0;
bool logincheck[3]{ false, false, false };
bool Initcheck = false;

