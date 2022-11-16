#pragma once
namespace PLAYER {
	enum Player_State {
		IDLE,
		LEFT,
		RIGHT,
		JUMP,
		DEAD
	};
}

namespace OBECJT
{
	enum Objet_State {
		Fire,
		Floor,
		Door,
		Pattern,
		Player
	};
}