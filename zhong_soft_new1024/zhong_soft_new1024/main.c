#include "func.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>

char *permissionMessg[] = {
	" 1.�����û�",
	" 2.ɾ���û�",
	" 3.�޸��û�",
	" 4.��ѯ�û�",
	" 5.���ӽ�ɫ",
	" 6.ɾ����ɫ",
	" 7.�޸Ľ�ɫ",
	" 8.��ѯ��ɫ",
	" 9.�޸�Ȩ��",
	"10.��ѯȨ��",
};

void main()
{

	logM *pSend = logToSystem();
	logM pSendRe;

	memset(&pSendRe, 0, sizeof(pSendRe));

	//printf("������Ϣȷ��:%d %s\n", pSend->headFlag[0], pSend->userMessage);//���Դ������ݵ���ȷ��
	SOCKET sockClient = connectToServer();							//���ӵ�������
	send(sockClient, (const char *)pSend, sizeof(*pSend), 0);		//��������

	recv(sockClient, (char *)&pSendRe, sizeof(pSendRe), 0);			//���ܷ��������ص�����
	//printf("������Ϣȷ��:%d   %s\n", pSendRe.headFlag[0], pSendRe.userMessage);//���Դ������ݵ���ȷ��
	strcpy(userInfo, pSendRe.userMessage);

	processServerReMsg(&pSendRe, sockClient);

	//�ر�socket
	closesocket(sockClient);
	WSACleanup();

	system("pause");
}