#pragma once
class Object {
public:
	Object() = default;
	Object(short id, short x, short y, short xs, short ys) //������ ���� ������
		: id{ id }, x{ x }, y{ y }, x_size{ xs }, y_size{ ys } {
	};

	short id{}; // id��
	short x{}; // x��ǥ
	short y{}; // y��ǥ
	short x_size{}; // x������
	short y_size{}; // y������
	RECT CollidBox;	//�浹�� ���� �ڽ� ������
};

