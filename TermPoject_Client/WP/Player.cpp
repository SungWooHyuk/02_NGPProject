#include "Player.h"
Player::Player() {
	x =0; 
	y =0;   
}
Player::Player(int id, int x, int y, int xs, int ys, int state)
	: Object(id, x, y, xs, ys), state_type{ state } { } 

int Player::getId() {
	return id;
}