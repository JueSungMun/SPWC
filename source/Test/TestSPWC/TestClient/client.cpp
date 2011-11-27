#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
void ErrorHandling(char* message);
void Mousecontrol(POINT p);
void MouseControl2(POINT p);

void MouseControl(POINT p)
{
	int px,py;
	SetCursorPos(p.x,p.y); //���콺 ��ġ�� �̵���Ű��
	while(1){
		GetCursorPos(&p);	//���콺 ��ġ��������
		//printf("%d, %d \n", p.x, p.y); //���콺��ġ���
		px=p.x;
		py=p.y;

		/*���콺 ���� -> ������, ������ -> ����*/
		if(px==(GetSystemMetrics(SM_CXVIRTUALSCREEN)-1))
			SetCursorPos(1,py); //���콺 ��ġ�� �̵���Ű��
		if(px==0)
			SetCursorPos(GetSystemMetrics(SM_CXVIRTUALSCREEN)-2,py); //���콺 ��ġ�� �̵���Ű��

		/*���콺 ���� -> �Ʒ���, �Ʒ��� -> ����*/
		if(GetSystemMetrics(SM_CXVIRTUALSCREEN)-GetSystemMetrics(SM_CXSCREEN) > px){
			if(py==(GetSystemMetrics(SM_CYSCREEN)-1))
				SetCursorPos(px,1); //���콺 ��ġ�� �̵���Ű��
			if(py==0)
				SetCursorPos(px,GetSystemMetrics(SM_CYSCREEN)-2); //���콺 ��ġ�� �̵���Ű��
		}else{
			if(py==(GetSystemMetrics(SM_CYVIRTUALSCREEN)-1))
				SetCursorPos(px,1); //���콺 ��ġ�� �̵���Ű��
			if(py==0)
				SetCursorPos(px,GetSystemMetrics(SM_CYVIRTUALSCREEN)-2); //���콺 ��ġ�� �̵���Ű��
		}
	}
}

void MouseControl2(POINT p)
{
	SetCursorPos(p.x,p.y); //���콺 ��ġ�� �̵���Ű��

}
int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	char message[30];
	int strLen;
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
		POINT pon;
		strLen = recv(hSocket, message, sizeof(pon), 0);	//recv�Լ� ȣ���� ���ؼ� �����κ��� ���۵Ǵ� �����͸� ��
				
		memcpy(&pon,message,sizeof(POINT));
		if(strLen == -1){
			ErrorHandling("read() error!");
			break;
		}
		printf("Message from server: %d, %d  \n", pon.x,pon.y);
		MouseControl2(pon);
		if(pon.x == 0 && pon.y ==0) break;
	}
	closesocket(hSocket);
	WSACleanup();	//���� ���̺귯���� ����
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

