#pragma once
class Object {
public:
	Object() = default;
	Object(short id, short x, short y, short xs, short ys);

	short id{}; // id��
	short x{}; // x��ǥ
	short y{}; // y��ǥ
	short x_size{}; // x������
	short y_size{}; // y������ 
};