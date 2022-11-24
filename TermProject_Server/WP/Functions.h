#pragma once
#include "protocol.h" 
#include "State.h"
class Object {
public:
	Object() = default;
	Object(short id, short x, short y, short xs, short ys) //셋팅을 위한 생성자
		: id{ id }, x{ x }, y{ y }, x_size{ xs }, y_size{ ys } {
	};

	short id{}; // id값
	short x{}; // x좌표
	short y{}; // y좌표
	short x_size{}; // x사이즈
	short y_size{}; // y사이즈 
	RECT CollidBox;// 충돌 처리를 위한 박스 
	
};

class Player :public Object
{
public:
	Player() = default;

	Player(short id, short x, short y, short xs, short ys, PLAYER::Player_State state, bool visible)
		: Object(id, x, y, xs, ys), state_type{ state }, visible{ visible } { };

	short getId() {
		return id;
	};

public:
	bool visible = false;
	PLAYER::Player_State state_type{}; // 상태에 대한 type  

};

