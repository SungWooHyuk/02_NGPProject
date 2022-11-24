#pragma once
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
	RECT CollidBox;	//충돌을 위한 박스 생성함
};

