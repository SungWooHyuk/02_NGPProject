#pragma once
#include "Object.h"
class Player :public Object
{
public:
	Player(); 
	Player(short id, short x, short y, short xs, short ys, short state);
	short getId();
public:
	short state_type{}; // 상태에 대한 type  
};


