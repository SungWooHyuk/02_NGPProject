#pragma once
#include "Object.h"
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


