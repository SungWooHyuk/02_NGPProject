#pragma once 
#include "Global.h" 
#include "Image.h" // 이미지 로드 헤더
#include "Object.h" 
#include "H_Fire.h"  
#include "W_Fire.h"
#include "Floor.h"
#include "Thron.h" 
#include "Pattern.h"
#include "Player.h" 
#include "protocol.h" 

Player playerStatus[MAXCLIENT]; // 플레이어 상태       
Object floorStatus[FLOORCNT]; // 바닥상태    
Object FireStatus[FIRECNT]; // 불 상태       
Object W_FireStatus[FIRECNT]; // 가로 불 상태
Object ThornStatus[THORNCNT]; // 가시 상태 
Object PatternStatus[PATTERNCNT]; // 패턴 상태 
Object gs_PatternStatus[PATTERNCNT]; // 흑백 패턴 상태   
Object ButtonStatus[BUTTONCNT];
Object doorstatus; //문의 상태      

LOGIN_PACKET login_info;   
INIT_PACKET  Init_info;		//3명 접속 후 패턴과 버튼의 초기 데이터 
OBJECT_UPDATE_PACKET update_info; //업데이트되는 모든 내용 
KEYINPUT_PAKCET keyinput_info; //플레이어가 입력한 키 값을 server로 넘겨줌 
void State(); // 지금 모든 오브젝트들 상태를 이 함수에서 변경 