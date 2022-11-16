#pragma once
#include "protocol.h" 

class Object {
public:
	Object() = default;
	//server에서 셋팅을 위한 생성자 
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
	//server에서 첫 위치 셋팅할 때 생성자로 만들어서 쓸 것임 
	Player(short id, short x, short y, short xs, short ys, short state)
		: Object(id, x, y, xs, ys), state_type{ state } {

	}

	//id값이 무엇인지 확인하기 위함 
	short getId() {
		return id;
	}

public:
	//초기에는 id는 0으로 stat_type는 IDLE로 기본값 설정  

	short state_type{}; // 상태에 대한 type 
};

