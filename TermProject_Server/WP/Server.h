#pragma once 
#include "Global.h" 
#include "protocol.h"
#include "Functions.h"
#include "State.h"

Player playerStatus[MAXCLIENT]; // �÷��̾� ����       
Object floorStatus[FLOORCNT]; // �ٴڻ���    
Object H_FireStatus[FIRECNT]; // �� ����       
Object W_FireStatus[FIRECNT]; // ���� �� ����
Object ThornStatus[THORNCNT]; // ���� ���� 
Object PatternStatus[PATTERNCNT]; // ���� ���� 
Object gs_PatternStatus[PATTERNCNT]; // ��� ���� ����   
Object ButtonStatus[BUTTONCNT];
Object doorstatus; //���� ����  

void State(); // ���� ��� ������Ʈ�� ���¸� �� �Լ����� ���� 
void SettingObjLocation(); //�ʱ� ��� objet���� ��ġ ���� 