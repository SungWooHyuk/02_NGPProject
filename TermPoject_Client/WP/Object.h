#pragma once
class Object {
public:
	Object() = default;
	Object(short id, short x, short y, short xs, short ys);

	short id{}; // id값
	short x{}; // x좌표
	short y{}; // y좌표
	short x_size{}; // x사이즈
	short y_size{}; // y사이즈 
};