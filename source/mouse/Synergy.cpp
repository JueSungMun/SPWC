// Synergy.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include <stdio.h>
#include <windows.h>
#include <tchar.h>


int main(int argc, char **argv)
{
	POINT p;
	int px,py;
	// GetSystemMetrics(SM_CXSCREEN) �Լ��� ���� ����� ������ �о�´�
	// GetSystemMetrics(SM_CMONITORS) ����� ����� ������ �����´�. ��������ȵǸ�0 ������ϳ���1 �����̸�2
	
	while(1){
		GetCursorPos(&p);	//���콺 ��ġ��������
		//printf("%d, %d \n", p.x, p.y); //���콺��ġ���
		px=p.x;
		if(px==(GetSystemMetrics(SM_CXVIRTUALSCREEN)-1))
			SetCursorPos(1,p.y); //���콺 ��ġ�� �̵���Ű��
		if(px==0)
			SetCursorPos(GetSystemMetrics(SM_CXVIRTUALSCREEN)-2,p.y); //���콺 ��ġ�� �̵���Ű��


		py=p.y;
		
	}
	return 0;
	
}