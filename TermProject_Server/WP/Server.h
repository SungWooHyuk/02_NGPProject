#pragma once 
#include "Global.h" 
#include "protocol.h"
#include "Functions.h"
#include "State.h"

Player playerStatus[MAXCLIENT]; // 플레이어 상태       
Object floorStatus[FLOORCNT]; // 바닥상태    
Object H_FireStatus[FIRECNT]; // 불 상태       
Object W_FireStatus[FIRECNT]; // 가로 불 상태
Object ThornStatus[THORNCNT]; // 가시 상태 
Object PatternStatus[PATTERNCNT]; // 패턴 상태 
Object gs_PatternStatus[PATTERNCNT]; // 흑백 패턴 상태   
Object ButtonStatus[BUTTONCNT];
Object doorstatus; //문의 상태  

void State(); // 지금 모든 오브젝트들 상태를 이 함수에서 변경 
void SettingObjLocation(); //초기 모든 objet들의 위치 설정 