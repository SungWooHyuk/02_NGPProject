#pragma once
#include <atlimage.h>

CImage PlayerImg;
CImage PlayerImg_2;
CImage PlayerImg_3;
CImage PlayerImg_j;
CImage PlayerImg_j_2;
CImage PlayerImg_j_3;
CImage BackGround;
CImage FloorImg;   
CImage ThronImg;
CImage Fire;
CImage Fire2;
CImage PatternImg;
CImage Pattern2;
CImage Pattern3;
CImage Pattern4;
CImage Pattern5;

//흑백이미지 
CImage GS_Pattern;
CImage GS_Pattern2;
CImage GS_Pattern3;
CImage GS_Pattern4;
CImage GS_Pattern5;

//문
CImage CloseDoor;

//gamemode
CImage GameOver;
CImage GameClear;
CImage Lobby;

//버튼 
CImage ButtonUP;
CImage ButtonDown;

void Image()
{
	BackGround.Load(L"Image/background.png");
	PlayerImg.Load(L"Image/Player.png");
	PlayerImg_2.Load(L"Image/Player2.png");
	PlayerImg_3.Load(L"Image/Player3.png");
	PlayerImg_j.Load(L"Image/Player1_j.png");
	PlayerImg_j_2.Load(L"Image/Player2_j.png");
	PlayerImg_j_3.Load(L"Image/Player3_j.png");
	FloorImg.Load(L"Image/floor.png"); 
	ThronImg.Load(L"Image/thorn.png"); 
	Fire.Load(L"Image/fire1.png");
	Fire2.Load(L"Image/fire3.png");
	PatternImg.Load(L"Image/pattern1.png"); 
	Pattern2.Load(L"Image/pattern2.png");
	Pattern3.Load(L"Image/pattern3.png");
	Pattern4.Load(L"Image/pattern4.png");
	Pattern5.Load(L"Image/pattern5.png");


	GS_Pattern.Load(L"Image/gs_pattern1.png");
	GS_Pattern2.Load(L"Image/gs_pattern2.png");
	GS_Pattern3.Load(L"Image/gs_pattern3.png");
	GS_Pattern4.Load(L"Image/gs_pattern4.png");
	GS_Pattern5.Load(L"Image/gs_pattern5.png");

	CloseDoor.Load(L"Image/closeDoor.png");
	GameOver.Load(L"Image/GameOver.png");
	GameClear.Load(L"Image/GameClear.png");
	Lobby.Load(L"Image/Lobby.png");
	ButtonUP.Load(L"Image/buttonUp.png");
	ButtonDown.Load(L"Image/buttonDown.png");
}
