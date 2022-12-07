#pragma once
#include "protocol.h" 
#include "State.h"

class Object {
public:
	Object() = default;
	Object(short id, short x, short y, short xs, short ys) //������ ���� ������
		: id{ id }, x{ x }, y{ y }, x_size{ xs }, y_size{ ys } {
	};
	Object(short id, short x, short y, short xs, short ys, bool pv) //������ ���� ������ //�ٲ�
		: id{ id }, x{ x }, y{ y }, x_size{ xs }, y_size{ ys }, objectvisible{ pv } {
	};
	short id{}; // id��
	short x{}; // x��ǥ
	short y{}; // y��ǥ
	short x_size{}; // x������
	short y_size{}; // y������ 
	bool objectvisible; //�ٲ�
	RECT CollidBox;// �浹 ó���� ���� �ڽ� 
};

class Player :public Object
{
public:
	Player() = default;

	Player(short id, short x, short y, short xs, short ys, PLAYER::Player_State state, bool visible)
		: Object(id, x, y, xs, ys), state_type{ state }, visible{ visible } { };

public:
	bool visible = false;
	bool jump = false;
	char name[20];
	PLAYER::Player_State state_type{}; // ���¿� ���� type  
};

