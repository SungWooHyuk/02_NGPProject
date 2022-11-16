#pragma once
#include "protocol.h" 

class Object {
public:
	Object() = default;
	//server���� ������ ���� ������ 
	Object(short id, short x, short y, short xs, short ys)
		: id{ id }, x{ x }, y{ y }, x_size{ xs }, y_size{ ys } {
	}

public:
	short id{};
	short x{};
	short y{};
	short x_size{};
	short y_size{};
};

class Player : public Object {
public:
	Player() = default;
	//server���� ù ��ġ ������ �� �����ڷ� ���� �� ���� 
	Player(short id, short x, short y, short xs, short ys, short state)
		: Object(id, x, y, xs, ys), state_type{ state } {

	}

	//id���� �������� Ȯ���ϱ� ���� 
	short getId() {
		return id;
	}

public:
	//�ʱ⿡�� id�� 0���� stat_type�� IDLE�� �⺻�� ����  

	short state_type{}; // ���¿� ���� type 
};

