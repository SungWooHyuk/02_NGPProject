#pragma once 
#include "Global.h" 
#include "State.h" // �߰�
#include "Image.h" // �̹��� �ε� ���
#include "Object.h" 
#include "Player.h" 
#include "protocol.h" 
#include "resource.h"

Player playerStatus[MAXCLIENT]; // �÷��̾� ����       
Object floorStatus[FLOORCNT]; // �ٴڻ���    
Object FireStatus[FIRECNT]; // �� ����       
Object W_FireStatus[FIRECNT]; // ���� �� ����
Object ThornStatus[THORNCNT]; // ���� ���� 
Object PatternStatus[PATTERNCNT]; // ���� ���� 
Object gs_PatternStatus[PATTERNCNT]; // ��� ���� ����   
Object ButtonStatus[BUTTONCNT];
Object doorstatus; //���� ����   

OBJECT_UPDATE_PACKET update_info; //������Ʈ�Ǵ� ��� ���� 
KEYINPUT_PAKCET keyinput_info; //�÷��̾ �Է��� Ű ���� server�� �Ѱ��� 