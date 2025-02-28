#include "func.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>

char *permissionMessg[] = {
	" 1.增加用户",
	" 2.删除用户",
	" 3.修改用户",
	" 4.查询用户",
	" 5.增加角色",
	" 6.删除角色",
	" 7.修改角色",
	" 8.查询角色",
	" 9.修改权限",
	"10.查询权限",
};

void main()
{

	logM *pSend = logToSystem();
	logM pSendRe;

	memset(&pSendRe, 0, sizeof(pSendRe));

	//printf("发送信息确认:%d %s\n", pSend->headFlag[0], pSend->userMessage);//测试传输数据的正确性
	SOCKET sockClient = connectToServer();							//连接到服务器
	send(sockClient, (const char *)pSend, sizeof(*pSend), 0);		//发送数据

	recv(sockClient, (char *)&pSendRe, sizeof(pSendRe), 0);			//接受服务器返回的数据
	//printf("发送信息确认:%d   %s\n", pSendRe.headFlag[0], pSendRe.userMessage);//测试传输数据的正确性
	strcpy(userInfo, pSendRe.userMessage);

	processServerReMsg(&pSendRe, sockClient);

	//关闭socket
	closesocket(sockClient);
	WSACleanup();

	system("pause");
}