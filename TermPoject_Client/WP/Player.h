#pragma once
#include "Object.h"
class Player :public Object
{
public:
	Player(); 
	Player(int id, int x, int y, int xs, int ys, int state); 
	int getId();
private:
	int state_type{}; // 상태에 대한 type  
};


