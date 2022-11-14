#pragma once
class Object {
public:
	Object() = default;
	Object(int id, int x, int y, int xs, int ys);

	int id{}; // id값
	int x{}; // x좌표
	int y{}; // y좌표
	int x_size{}; // x사이즈
	int y_size{}; // y사이즈 
};