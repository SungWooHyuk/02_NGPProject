#pragma once 
#include "Global.h" 
#include "State.h" // 추가
#include "Image.h" // 이미지 로드 헤더
#include "Object.h" 
#include "Player.h" 
#include "protocol.h" 
#include "resource.h"

Player playerStatus[MAXCLIENT]; // 플레이어 상태       
Object floorStatus[FLOORCNT]; // 바닥상태    
Object FireStatus[FIRECNT]; // 불 상태       
Object W_FireStatus[FIRECNT]; // 가로 불 상태
Object ThornStatus[THORNCNT]; // 가시 상태 
Object PatternStatus[PATTERNCNT]; // 패턴 상태 
Object gs_PatternStatus[PATTERNCNT]; // 흑백 패턴 상태   
Object ButtonStatus[BUTTONCNT];
Object doorstatus; //문의 상태   

OBJECT_UPDATE_PACKET update_info; //업데이트되는 모든 내용 
KEYINPUT_PAKCET keyinput_info; //플레이어가 입력한 키 값을 server로 넘겨줌 