#include <windows.h>
#include <stdio.h>
#include <iostream>
HANDLE hThread[3];
HANDLE MyUpdateThread;
HANDLE hhThread[3];

int cnt{};
DWORD WINAPI MyThread(LPVOID arg) 
{ 
	
	 int m_id = (int)arg;

	while (1) {
		
		WaitForSingleObject(hhThread[m_id], INFINITE);
		//ResetEvent(MyUpdateThread); 
		printf("�ȳ��ϼ��� %d��\n", m_id);

		SetEvent(hThread[m_id]);
		ResetEvent(hhThread[m_id]);

		
	}
	return 0;
}

DWORD WINAPI UpdateThread(LPVOID arg)
{
	DWORD retval;
	while (1) {

		//WaitForSingleObject(hThread[0], INFINITE);

		retval = WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

		//Update();
		//isCollision();
		//send;

		printf("������Ʈ\n");
		
		ResetEvent(hThread[0]);
		ResetEvent(hThread[1]);
		ResetEvent(hThread[2]);
		SetEvent(hhThread[0]);
		SetEvent(hhThread[1]);
		SetEvent(hhThread[2]);

		//ResetEvent(MyUpdateThread);
	}
	return 0;
}
int main(int argc, char* argv[])
{
	// 2��° ���ڰ� TRUE : ����
	// 3��° ���ڰ� FALSE : ���ȣ 
	hThread[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
	hThread[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	hThread[2] = CreateEvent(NULL, TRUE, FALSE, NULL); 

	hhThread[0] = CreateEvent(NULL, TRUE, TRUE, NULL);
	hhThread[1] = CreateEvent(NULL, TRUE, TRUE, NULL);
	hhThread[2] = CreateEvent(NULL, TRUE, TRUE, NULL);

	//���ȣ�� �������
	MyUpdateThread = CreateEvent(NULL, TRUE, TRUE, NULL); //��ȣ�� �������  
	//���ȣ : ��ȣ�� ������ ���� ��ٸ��ٴ� ���� �ǹ��Ѵ�. 
	//SetEvent(MyUpdateThread);
	HANDLE rr[3];
	rr[0] = CreateThread(NULL, 0, MyThread, (LPVOID)0, 0, NULL);
	rr[1] = CreateThread(NULL, 0, MyThread, (LPVOID)1, 0, NULL);
	rr[2] = CreateThread(NULL, 0, MyThread, (LPVOID)2, 0, NULL);
	HANDLE tt = CreateThread(NULL, 0, UpdateThread, NULL, 0, NULL);

	while (1)
	{
		
	}

	CloseHandle(rr[0]);
	CloseHandle(rr[1]);
	CloseHandle(rr[2]);
	CloseHandle(tt);  

	// �̺�Ʈ ����
	CloseHandle(hThread);
	CloseHandle(MyUpdateThread);  
	  
	return 0;
}