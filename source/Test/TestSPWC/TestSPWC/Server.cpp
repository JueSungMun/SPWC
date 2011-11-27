#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <tchar.h>
#include <process.h>

#define MAX_THREADS 1

void ErrorHandling(char* message);

DWORD cntOfThread = 0;

//Ŭ���̾�Ʈ�� ���� �����ϴ� �Լ�

unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
	SOCKET clnt;
	POINT pon;
	clnt = (SOCKET)lpParam;
	long time = GetCurrentTime();
	while(1){
		
		GetCursorPos(&pon);	
		if(time + 10 < GetCurrentTime() ){
			send(clnt, (char*)&pon, sizeof(pon), 0);
			if(pon.x == 0 && pon.y ==0) break;
			time = GetCurrentTime();
		}		
	}
	//MouseControl();
	closesocket(clnt);
	
	return 0;
}


int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadId[MAX_THREADS];		//������ ���̵�
	HANDLE hThread[MAX_THREADS];		//������ �ڵ�
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int portNum = 8080;
	printf("�������α׷��� �����մϴ�.\n");
	
	int szClntAddr;
	/*
	if(argc!=2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	*/

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
		hThread[cntOfThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc,(LPVOID) hClntSock,0,(unsigned *) &dwThreadId[cntOfThread]);
		if(hThread[cntOfThread] == NULL)
		{
			_tprintf(_T("�����带 �����Ҽ��� �����ϴ�.\n"));
		}
	}
	
	closesocket(hServSock);
	WSACleanup();	//���α׷��� �����ϱ����� �ʱ�ȭ�� ���� ���̺귯���� ����
	CloseHandle(hThread[cntOfThread]);
	return 0;
	
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

