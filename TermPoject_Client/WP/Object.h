#pragma once
class Object {
public:
	Object() = default;
	Object(int id, int x, int y, int xs, int ys);

	int id{}; // id��
	int x{}; // x��ǥ
	int y{}; // y��ǥ
	int x_size{}; // x������
	int y_size{}; // y������ 
};