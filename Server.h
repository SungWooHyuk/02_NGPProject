#pragma once 
#include "Global.h" 
#include "protocol.h"
#include "Functions.h"
#include "Image.h" // �̹��� �ε� ���


Player playerStatus; // �÷��̾� ����  
Object floorStatus[FLOORCNT]; // �ٴڻ���    
Object FireStatus[FIRECNT]; // �� ����       
Object W_FireStatus[FIRECNT]; // ���� �� ����
Object ThornStatus[THORNCNT]; // ���� ���� 
Object PatternStatus[PATTERNCNT]; // ���� ���� 
Object gs_PatternStatus[PATTERNCNT]; // ��� ���� ����   
Object doorstatus; //���� ����      

void State(); // ���� ��� ������Ʈ�� ���¸� �� �Լ����� ���� 
void SettingObjLocation(); //�ʱ� ��� objet���� ��ġ ���� 