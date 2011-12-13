#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <tchar.h>
#include <process.h>

#define MAX_THREADS 1

void ErrorHandling(char* message);

DWORD cntOfThread = 1;
LONG positionMouse[3]; //Ŭ���̾�Ʈ�� ���� �����ϴ� �Լ� (Ŭ���̾�Ʈ�� ���� ��ų �Լ�, ������ ���콺 ������ ������ ����)
//HANDLE hEvent;
int  nClickNum=0;

unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
	SOCKET clnt;
	POINT pon;
	clnt = (SOCKET)lpParam;
	long time = GetCurrentTime();
	
	while(1){
		GetCursorPos(&pon);	
		positionMouse[0] = pon.x;
		positionMouse[1] = pon.y;
		if(time + 10 < GetCurrentTime())
		{
			//10mm ������ ���� ������ ������.
			//WaitForSingleObject (hMutex, INFINITE);
			//SetEvent(hEvent);
			send(clnt, (char*)positionMouse, sizeof(positionMouse), 0);
			if(pon.x == 0 && pon.y ==0) break;
			time = GetCurrentTime();			
			nClickNum = 0;
			positionMouse[2] = 0;
			
			//SetEvent(hEvent);
		}
		
	}
	
	closesocket(clnt);
	return 0;
}

unsigned int WINAPI function(LPVOID lpParam)
{
	INPUT_RECORD Inrec; //���콺�� Ű���� �̺�Ʈ�� ������ ����ü
	DWORD evRead;
	HANDLE hStdIn=NULL;
	DWORD dwMode;
	
	while(1)
	{
		
		//printf("�ǳ�?\n");
		//GetConsoleMode(hStdIn, &dwMode);
		//FlushConsoleInputBuffer(hStdIn);
		//WaitForSingleObject(hEvent,INFINITE);
		hStdIn = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hStdIn, &dwMode);
		if(hStdIn!=NULL)
		{
			if(!(dwMode & ENABLE_MOUSE_INPUT))
			{
				if(TRUE != SetConsoleMode( hStdIn, dwMode | ENABLE_MOUSE_INPUT))
				{
					return 0;
				}
			}

			PeekConsoleInput(hStdIn, &Inrec, 1, &evRead);
			
			if( evRead>0) 
			{
				ReadConsoleInput(hStdIn, &Inrec, 1, &evRead);

				switch (Inrec.EventType)
				{
					/*
					case MOUSE_EVENT:
						MouseEventProc(Inrec[i%100].Event.MouseEvent);
						break;
					*/
					case MOUSE_EVENT:
					{
						//FROM_LEFT_1ST_BUTTON_PRESSED �̰�1
						//FROM_LEFT_2ND_BUTTON_PRESSED �̰�2
						
						if(FROM_LEFT_1ST_BUTTON_PRESSED & Inrec.Event.MouseEvent.dwButtonState) //����Ŭ�� �Է�ó��
							{
								nClickNum |= 0x01;	
								//SetEvent(hEvent);
								printf("���콺 ���� Ŭ������\n");	
								//break;
							}
						if(FROM_LEFT_2ND_BUTTON_PRESSED & Inrec.Event.MouseEvent.dwButtonState) // �� Ŭ�� �Է�ó��
							{
								nClickNum |= 0x04;
								//printf("%d\n",FROM_LEFT_2ND_BUTTON_PRESSED+Inrec.Event.MouseEvent.dwButtonState);
								//SetEvent(hEvent);
								printf("��Ŭ������\n");	
								//break;
							}
						if(RIGHTMOST_BUTTON_PRESSED & Inrec.Event.MouseEvent.dwButtonState) // ������ Ŭ�� �Է�ó��
							{
								nClickNum |= 0x02;
								//printf("%d\n",RIGHTMOST_BUTTON_PRESSED+Inrec.Event.MouseEvent.dwButtonState);
								//SetEvent(hEvent);
								printf("���콺 ������ Ŭ������\n");			
								//break;
							}
						
						if(DOUBLE_CLICK & Inrec.Event.MouseEvent.dwEventFlags) //
							{
								nClickNum |= 0x08;
								//SetEvent(hEvent);
								//printf("%d\n",DOUBLE_CLICK+Inrec.Event.MouseEvent.dwEventFlags);
								printf("���콺 ���� Ŭ������\n");
								//break;
							}
						
						
						positionMouse[2] = nClickNum;
						//ResetEvent(hEvent);	
						break; 
					}
					
					case KEY_EVENT:
					{
						_tprintf(_T("No key\n"));
						exit(1);
						break;
					}
				}// Switch�� �ݱ�
			}// if�� �ݱ�
			//WriteConsoleInput(hStdIn, &Inrec, 1, &evRead);
			//hStdIn = NULL;
			//SetConsoleMode(hStdIn,NULL);
						
		}//if�� �ݱ�
	}//while �� �ݱ�
		
	//gotoxy(0, 0);
	//printf("%d,%d",nClickNum&0x01,  nClickNum&0x02);
	//printf("nClickNum : %d\n",nClickNum);
	return 0;	
}

/*
VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
{
	#ifndef MOUSE_HWHEELED
	#define MOUSE_HWHEELED 0x0008
	#endif
		printf("Mouse event: ");
    
    switch(mer.dwEventFlags)
    {
		int nClickNum;
        case 0:
            printf("button press\n");
            break;
        case DOUBLE_CLICK:
			nClickNum |= 0x01;
			positionMouse[2] = nClickNum;
			printf("���콺 ���� Ŭ������\n");
			WaitForSingleObject (hEvent, INFINITE);
            //printf("double click\n");
            break;
        case MOUSE_HWHEELED:
            printf("horizontal mouse wheel\n");
            break;
        case MOUSE_MOVED:
            printf("mouse moved\n");
            break;
        case MOUSE_WHEELED:
            printf("vertical mouse wheel\n");
            break;
        default:
            printf("unknown\n");
            break;
    }
}
*/
int _tmain(int argc, TCHAR* argv[])
{
//	hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
	DWORD dwThreadId[MAX_THREADS];		//������ ���̵�
	HANDLE hThread[MAX_THREADS];		//������ �ڵ�
	
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	
	int portNum = 8080;
	printf("�������α׷��� �����մϴ�.\n");
	int szClntAddr;
	

	if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0)	//���� ���̺귯���� �ʱ�ȭ
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);	//���� ����
	if(hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(portNum);

	if(bind(hServSock, (SOCKADDR*) &servAddr, sizeof(servAddr))==SOCKET_ERROR)	//���Ͽ� IP�ּ�, PORT��ȣ�� �Ҵ�
		ErrorHandling("bind() error");

	if(listen(hServSock, 5) == SOCKET_ERROR)	//listen �Լ� ȣ���� ���ؼ�, ������ ������ ���� �������� �ϼ�
		ErrorHandling("listen() error");
	
	szClntAddr = sizeof(clntAddr);
	while(1)
	{

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);	//Ŭ���̾�Ʈ�� ���� ��û�� �����ϱ� ���ؼ� accept�Լ��� ȣ��
		if(hClntSock == INVALID_SOCKET)
			ErrorHandling("accept() error");
		hThread[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc,(LPVOID) hClntSock,0,(unsigned *) &dwThreadId[0]);
		if(hThread[0] == NULL)
		{
			_tprintf(_T("�����带 �����Ҽ��� �����ϴ�.\n"));
		}
		hThread[1] = (HANDLE)_beginthreadex(NULL, 0, function, (LPVOID) hClntSock,0,(unsigned *) &dwThreadId[1]);
		if(hThread[1] == NULL)
		{
			_tprintf(_T("�����带 �����Ҽ��� �����ϴ�.\n"));
		}
		
	}
	
	closesocket(hServSock);
	WSACleanup();	//���α׷��� �����ϱ����� �ʱ�ȭ�� ���� ���̺귯���� ����
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	//CloseHandle(hEvent);
	return 0;
	
}


void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

