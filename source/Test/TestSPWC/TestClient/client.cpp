#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>
#include <tchar.h>

void ErrorHandling(char* message);
LONG mousePosition[3];
int nClickNum;

unsigned int WINAPI MouseControl(LPVOID param)
{
	POINT p;
	//mousePosition = (LONG*) &param;
	p.x = mousePosition[0];
	p.y = mousePosition[1];
	SetCursorPos(p.x,p.y); //���콺 ��ġ�� �̵���Ű��
	/*���콺 ���� -> ������, ������ -> ����*/
	if(p.x==(GetSystemMetrics(SM_CXVIRTUALSCREEN)-1))
			SetCursorPos(1,p.y); //���콺 ��ġ�� �̵���Ű��
	if(p.x==0)
			SetCursorPos(GetSystemMetrics(SM_CXVIRTUALSCREEN)-2,p.y); //���콺 ��ġ�� �̵���Ű��
	/*���콺 ���� -> �Ʒ���, �Ʒ��� -> ����*/
	if(GetSystemMetrics(SM_CXVIRTUALSCREEN)-GetSystemMetrics(SM_CXSCREEN) > p.x){
			if(p.y==(GetSystemMetrics(SM_CYSCREEN)-1))
				SetCursorPos(p.x,1); //���콺 ��ġ�� �̵���Ű��
			if(p.y==0)
				SetCursorPos(p.x,GetSystemMetrics(SM_CYSCREEN)-2); //���콺 ��ġ�� �̵���Ű��
		}else{
			if(p.y==(GetSystemMetrics(SM_CYVIRTUALSCREEN)-1))
				SetCursorPos(p.x,1); //���콺 ��ġ�� �̵���Ű��
			if(p.y==0)
				SetCursorPos(p.x,GetSystemMetrics(SM_CYVIRTUALSCREEN)-2); //���콺 ��ġ�� �̵���Ű��
	}
	
	return 0;
}

unsigned int WINAPI function(LPVOID param)
{
	int nClickNum = mousePosition[2];	
	if(nClickNum != -1)
	{
		switch (nClickNum)
		{
			case 1:
			{	
				mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
				mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
				printf("���콺 ���� Ŭ��\n");
				break; 
			}
			case 4:
			{
				printf("��Ŭ��\n");
				break;
			}
			case 2:
			{
				mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
				mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
				printf("���콺 ������ Ŭ��\n");
				break;
			}
		}
	}
	
	  //gotoxy(0, 0);
	  //printf("%d,%d",nClickNum&0x01,  nClickNum&0x02);
		//printf("nClickNum : %d\n",nClickNum);
		
	return 0;
	
}

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	int strLen;
	DWORD dwThreadId[2];
	HANDLE hThread[2];
	//POINT pon;

	if(argc!=3)
	{
		printf("Usage : %s <IP>\n", argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0)	//���� ���̺귯���� �ʱ�ȭ
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);	//���� ����
	if(hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR) //������ ������ �������� ������ ���� ��û
		ErrorHandling("connect() error");
	
	while(1)
	{
		strLen = recv(hSocket, (char*)mousePosition, sizeof(mousePosition), 0);	//recv�Լ� ȣ���� ���ؼ� �����κ��� ���۵Ǵ� �����͸� ��
		//memcpy(&pon,message,sizeof(POINT));
		if(strLen == -1){
			ErrorHandling("read() error!");
			break;
		}
		//printf("Message from server: %d, %d  \n", pon.x,pon.y);
		
		hThread[0] = (HANDLE)_beginthreadex(NULL, 0, MouseControl, (LPVOID)mousePosition,0,(unsigned *) &dwThreadId[0]);
		hThread[1] = (HANDLE)_beginthreadex(NULL, 0, function, (LPVOID)mousePosition,0,(unsigned *) &dwThreadId[1]);
		//MouseControl2(pon);
		//if(pon.x == 0 && pon.y ==0) break;
	}
	closesocket(hSocket);
	WSACleanup();	//���� ���̺귯���� ����
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

