#include "func.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void enterReturnMenu()
{
	fflush(stdin);
	printf("\n");
	system("pause");
	system("color 0A");
	system("cls");
}

/********************************************************************
*登录到服务器
********************************************************************/
SOCKET  connectToServer()
{
	WSADATA wsaData;
	SOCKET sockClient;			//客户端Socket
	SOCKADDR_IN addrServer;		//服务端地址
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sockClient = socket(AF_INET, SOCK_STREAM, 0);	//新建客户端socket
	//ipv4 internet协议； TCP连接，提供序列化的/可靠的/双向字节流，支持带外数据传输；协议类型

	//定义要连接的服务端地址
	addrServer.sin_addr.S_un.S_addr = inet_addr("192.168.229.129");
	addrServer.sin_family = AF_INET; //AF_INET和PF_INET值是一致的。表示IPV4
	addrServer.sin_port = htons(10002);		//连接端口10002

	//连接到服务端
	int ret = connect(sockClient, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
	if (ret)
	{
		system("cls");
		system("color 0C");
		printf("\n\n**************************** 错误 ****************************\n");
		printf("服务器连接错误！请检查网络后重启。\n");
		Sleep(3000);
		exit(-1);
	}
	return sockClient;
}

/************************************************************************/
/* 遍历用户信息判断非字母非数字                                                                     */
/************************************************************************/
int travUserMesg(char *userMessage)    //遍历用户信息
{
	if (userMessage == NULL)
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("用户信息指针为空！");
		enterReturnMenu();

		return -1;
	}
	else if (strlen(userMessage) > 8 || strlen(userMessage) <= 0)
	{
		return -2;
	}

	while (*userMessage != '\0')							//遍历用户名
	{
		if (isalpha(*userMessage) || isdigit(*userMessage)) //判断字母和数字
		{
			userMessage++;
		}
		else
		{
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("用户信息仅限于纯数字、纯字母或两者组合!");
			enterReturnMenu();
			return -1;
		}
	}
	return 1;
}


/********************************************************************
*初始化给服务发送消息的结构体
********************************************************************/
void initStruct(logM * plogM)
{
	memset(plogM->headFlag, 0, sizeof(plogM->headFlag));
	memset(plogM->userMessage, 0, sizeof(plogM->userMessage));
}
/************************************************************************/
/* 初始化用户所有信息结构体                                                                     */
/************************************************************************/
void initUserALLMsgStruct(userMsg * pUserMsg)
{
	memset(pUserMsg->userName, 0, sizeof(pUserMsg->userName));
	memset(pUserMsg->del, 0, sizeof(pUserMsg->del));

	memset(pUserMsg->mail, 0, sizeof(pUserMsg->mail));
	memset(pUserMsg->job, 0, sizeof(pUserMsg->job));
	memset(pUserMsg->createPerson, 0, sizeof(pUserMsg->createPerson));
	memset(pUserMsg->createTime, 0, sizeof(pUserMsg->createTime));
	memset(pUserMsg->role, 0, sizeof(pUserMsg->role));
	memset(pUserMsg->password, 0, sizeof(pUserMsg->password));
}
/************************************************************************/
/* 处理服务器返回的headflag信息，根据返回消息确认处理方法                     */
/************************************************************************/
void processServerReMsg(logM *receiveServerMsg, SOCKET sockClient)
{
	logM *pSend = receiveServerMsg;
	SOCKET sok = sockClient;

	if (1 == pSend->headFlag[0])  //判断服务器传回消息
	{
		system("cls");
		printf("\n\n------------------------------------------------------------------------");

		printf("\n\n               登录成功……");
		printf("\n\n------------------------------------------------------------------------");

		Sleep(2000);
		system("cls");

		int choose = -1;		 //登陆成功之后调用系统管理菜单的所有功能
		while (choose)			 //循环用户选择 1.用户管理  2.角色管理  3.权限管理 
		{
			manageMenu();
			fflush(stdin);

			printf("\n\n------------------------------------------------------------------------");
			printf("\n请选择功能:");
			scanf("%d", &choose);
			system("cls");
			if (getchar() != '\n')
			{
				system("color 0C");
				printf("\n\n******************************* 错误 *******************************\n");
				printf("请重新输入下列选项编号：");
				enterReturnMenu();
				continue;
			}


			if (choose == 1 && strncmp(userInfo, "0000", 4))			//用户管理
			{
				userManage(sok);
			}

			else if (2 == choose && strncmp(userInfo + 4, "0000", 4))   //角色管理
			{
				roleManage(sok);
			}

			else if (3 == choose && strncmp(userInfo + 8, "00", 2))
			{
				permissionManage(sok);
			}
			else if (0 != choose)
			{
				system("color 0C");
				printf("\n\n******************************* 错误 *******************************\n");
				printf("请重新输入下列选项编号：");
				enterReturnMenu();
			}
		}
	}
	else  //服务器传回消息失败
	{
		system("color 0C");
		printf("\n\n******************************* 错误 *******************************\n");

		printf("\n\t\t登录失败！");
		printf("\n\n******************************* 错误 *******************************");
		enterReturnMenu();
		Sleep(2000);
		exit(0);
	}
}


////////////////////////////////用户管理//////////////////////////////////
void addUser(SOCKET sockClient)
{
	SOCKET sok = sockClient;
	userMsg *pUserMsg = (userMsg *)calloc(1, sizeof(userMsg));
	char *allMsg = (char *)calloc(1024, sizeof(char));

	logM *pRe = (logM *)calloc(1, sizeof(logM));
	//登陆之后服务器返回给客户端headflag[1]=1;给服务器发送增加的用户的信息

	logM *pFromServer = (logM *)calloc(1, sizeof(logM)); //临时变量，接受服务器返回的用户添加成功与否信息

	char *pum = NULL;
	initUserALLMsgStruct(pUserMsg);

	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** 用户管理系统 ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t> ● 用户管理\n");
	printf("\t\t> ● 增加用户\n");

	printf("------------------------------------------------------------------------\n");
	pRe->headFlag[0] = 2;	//用户管理
	pRe->headFlag[1] = 1;	//增加用户的标志
	while (1)
	{
		printf("\t请输入用户名<1-8位数字、字母或两者组合.>:");
		scanf("%s", pUserMsg->userName);

		pum = pUserMsg->userName;
		if (1 == travUserMesg(pum))
		{
			strcat(allMsg, pUserMsg->userName);
			strcat(allMsg, ":");
			break;
		}
		else
		{
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("输入格式不对，请重新输入：");
			enterReturnMenu();
			printf("\n\n------------------------------------------------------------------------\n");

		}

	}
	int index;
	while (1)
	{
		printf("\t请输入电话<例如 :18899997777>:");
		scanf("%s", pUserMsg->del);
		if (strlen(pUserMsg->del) == 11)
		{
			for (index = 0; index < 11; index++)
			{
				if (isdigit(pUserMsg->del[index]))
				{
					;
				}
				else
				{
					break;
				}
			}
			if (index == 11)
			{
				strcat(allMsg, pUserMsg->del);
				strcat(allMsg, ":");
				break;
			}
			else
			{
				system("color 0C");
				printf("\n\n******************************** 错误 **********************************\n");
				printf("输入格式不对，请重新输入：");
				enterReturnMenu();
				printf("\n\n------------------------------------------------------------------------\n");
			}
		}
		else
		{
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("输入格式不对，请重新输入：");
			enterReturnMenu();
			printf("\n\n------------------------------------------------------------------------\n");

		}
	}

	while (1)
	{
		printf("\t请输入邮箱<例如: china@xx.com>:");
		scanf("%s", pUserMsg->mail);
		char *p = strstr(pUserMsg->mail, "@");
		if (p)
		{
			char *tmp = pUserMsg->mail + strlen(pUserMsg->mail) - 4;
			if (strncmp(tmp, ".com", 4) == 0)
			{
				strcat(allMsg, pUserMsg->mail);
				strcat(allMsg, ":");
				break;
			}
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("输入格式不对，请重新输入：");
			enterReturnMenu();
			printf("\n\n------------------------------------------------------------------------\n");
		}
		else
		{
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("输入格式不对，请重新输入：");
			enterReturnMenu();
			printf("\n\n------------------------------------------------------------------------\n");
		}
	}

	while (1)
	{
		printf("\t请输入职位<1-15位字母.>:");
		scanf("%s", pUserMsg->job);
		int length = strlen(pUserMsg->job);
		if (length > 0 && length < 16)
		{
			for (index = 0; index < length; index++)
			{
				if (!isalpha(pUserMsg->job[index]))
				{
					break;
				}
			}
			if (index == length)
			{
				strcat(allMsg, pUserMsg->job);
				strcat(allMsg, ":");
				break;
			}
		}
		else
		{
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("输入格式不对，请重新输入：");
			enterReturnMenu();
			printf("\n\n------------------------------------------------------------------------\n");
		}

	}

	//获取创建者
	strcat(allMsg, userInfo + 10);
	strcat(allMsg, ":");

	//获取创建时间
	getSystemTime(pUserMsg->createTime);
	strcat(allMsg, pUserMsg->createTime);
	strcat(allMsg, ":");
	system("cls");

	logM *roleList = listExistRole(sok);
	if (getNameOrRoleInput(roleList->userMessage, pUserMsg->role) == 1)
	{
		strcat(allMsg, pUserMsg->role);
		strcat(allMsg, ":");
	}

	char password[100] = { 0 };
	while (1)
	{
		memset(pUserMsg->password, 0, sizeof(pUserMsg->password));
		memset(password, 0, sizeof(password));
		printf("\n------------------------------------------------------------------------\n");
		printf("\t请输入密码<1-8位数字、字母或它们的组合.>:");
		judgePasswd(password);
		pum = password;
		if (travUserMesg(pum) == 1)
		{
			printf("\n\t请再次确认密码:");
			judgePasswd(pUserMsg->password);
			pum = pUserMsg->password;
			if (travUserMesg(pum) == 1)
			{
				if (strcmp(password, pUserMsg->password) == 0)
				{
					strcat(allMsg, pUserMsg->password);
					break;
				}
				system("color 0C");
				printf("\n\n******************************** 错误 **********************************\n");
				printf("\t两次输入的密码不同！请检查重新输入！");
				enterReturnMenu();
				printf("\n\n------------------------------------------------------------------------\n");

			}
		}
		else
		{
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("\t密码输入格式不正确\n");
			enterReturnMenu();
			printf("\n\n------------------------------------------------------------------------\n");

		}
	}

	strcpy(pRe->userMessage, allMsg);

	//printf("\n发送用户信息确认\n增加用户标识：headFlag[1]:%d\n用户信息：%s\n", pRe->headFlag[1], pRe->userMessage);
	send(sockClient, (char *)pRe, sizeof(*pRe), 0);			//发送数据

	recv(sockClient, (char *)pFromServer, sizeof(*pFromServer), 0);			//接受服务器返回的数据
	//printf("received server :%d %s\n", pFromServer->headFlag[1], pFromServer->userMessage);

	if (1 == pFromServer->headFlag[1])
	{
		printf("\n\t添加用户成功!");
		enterReturnMenu();
	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\t添加用户失败！请检查您的输入，重新添加！");
		printf("\n\t【%s】\n", pFromServer->userMessage);
		enterReturnMenu();
	}

	free(pUserMsg);
	free(allMsg);
	free(pRe);
	free(pFromServer);
}
void deleteUser(SOCKET sockClient)
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//发送向服务其请求查询的消息
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//接受服务器回传的状态信息和用户信息
	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** 用户管理系统 ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t> ● 用户管理\n");
	printf("\t\t> ● 删除用户\n");

	printf("------------------------------------------------------------------------\n");

	logM *nameList = listExistUserName(soc);
	char queryName[20] = { 0 };
	if (getNameOrRoleInput(nameList->userMessage, queryName) == 1)
	{
		free(nameList);
		strcpy(plogM->userMessage, queryName);
	}


	plogM->headFlag[0] = 2;
	plogM->headFlag[1] = 2;
	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//发送数据
	//printf("\n发送用户名确认headFlag[1]:%d %s\n", plogM->headFlag[1], plogM->userMessage);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);		//接受服务器返回的数据
	//printf("received server :%d %s\n", plogMRe->headFlag[1], plogMRe->userMessage);

	if (2 == plogMRe->headFlag[1])
	{
		printf("\n\n------------------------------------------------------------------------\n");
		printf("\t删除成功！");
		enterReturnMenu();

	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\t删除失败！请检查您的操作！");
		printf("\n\t【%s】\n", plogMRe->userMessage);

		enterReturnMenu();
	}

	free(plogM);
	free(plogMRe);
}
void changeUser(SOCKET sockClient)
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//发送向服务其请求查询的消息
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//接受服务器回传的状态信息和用户信息

	userMsg *pUserMsg = (userMsg *)calloc(1, sizeof(userMsg));	//接受输入的修改信息
	char *allMsg = (char *)calloc(1024, sizeof(char));			//保存修改信息，

	//修改用户具体操作
	//列出所有用户名
	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** 用户管理系统 ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t> ● 用户管理\n");
	printf("\t\t> ● 修改用户\n");

	printf("------------------------------------------------------------------------\n");
	logM *nameList = listExistUserName(soc);
	if (getNameOrRoleInput(nameList->userMessage, pUserMsg->userName) == 1)
	{
		strcat(allMsg, pUserMsg->userName);
		strcat(allMsg, ":");
	}

	system("cls");

	int index;
	while (1)
	{
		printf("\n\n------------------------------------------------------------------------\n");
		printf("\t请输入电话:");
		scanf("%s", pUserMsg->del);
		if (strlen(pUserMsg->del) == 11)
		{
			for (index = 0; index < 11; index++)
			{
				if (isdigit(pUserMsg->del[index]))
				{
					;
				}
				else
				{
					break;
				}
			}
			if (index == 11)
			{
				strcat(allMsg, pUserMsg->del);
				strcat(allMsg, ":");
				break;
			}
			else
			{
				system("color 0C");
				printf("\n\n******************************** 错误 **********************************\n");
				printf("输入格式不对，请重新输入：\n");
				enterReturnMenu();
			}
		}
		else
		{
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("输入格式不对，请重新输入：\n");
			enterReturnMenu();
		}
	}

	while (1)
	{
		printf("\n\n------------------------------------------------------------------------\n");
		printf("\t请输入邮箱:");
		scanf("%s", pUserMsg->mail);
		char *p = strstr(pUserMsg->mail, "@");
		if (p)
		{
			char *tmp = pUserMsg->mail + strlen(pUserMsg->mail) - 4;
			if (strncmp(tmp, ".com", 4) == 0)
			{
				strcat(allMsg, pUserMsg->mail);
				strcat(allMsg, ":");
				break;
			}
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("输入格式不对，请重新输入：\n");
			enterReturnMenu();
		}
		else
		{
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("输入格式不对，请重新输入：\n");
			enterReturnMenu();
		}
	}

	while (1)
	{
		printf("\n\n------------------------------------------------------------------------\n");
		printf("\t请输入职位:");
		scanf("%s", pUserMsg->job);
		int length = strlen(pUserMsg->job);
		if (length > 0 && length < 20)
		{
			for (index = 0; index < length; index++)
			{
				if (!isalpha(pUserMsg->job[index]))
				{
					system("color 0C");
					printf("\n\n******************************** 错误 **********************************\n");
					printf("输入格式不对，请重新输入：\n");
					enterReturnMenu();
					break;
				}

			}
			if (index == length)
			{
				strcat(allMsg, pUserMsg->job);
				strcat(allMsg, ":");
				break;
			}
		}
		else
		{
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("输入格式不对，请重新输入：\n");
			enterReturnMenu();
		}
	}

	/*printf("\n\n\n\n       请输入创建者:");
	scanf("%s", pUserMsg->createPerson);

	strcat(allMsg, pUserMsg->createPerson);
	strcat(allMsg, ":");
	system("cls");

	printf("\n\n\n\n       请输入创建时间:");
	scanf("%s", pUserMsg->createTime);

	strcat(allMsg, pUserMsg->createTime);
	strcat(allMsg, ":");
	system("cls");*/

	//logM *roleList = listExistRole(soc);
	//char *p, *start;
	//while (1)
	//{
	//	p = start = roleList->userMessage;
	//	printf("\n\n------------------------------------------------------------------------\n");
	//	printf("\t请输入角色:");
	//	scanf("%s", pUserMsg->role);
	//	while (*p)
	//	{
	//		p = strchr(start, ':');
	//		if (strncmp(start, pUserMsg->role, p - start) == 0)
	//		{
	//			strcat(allMsg, pUserMsg->role);
	//			strcat(allMsg, ":");
	//			free(roleList);
	//			//system("cls");
	//			break;
	//		}
	//		p++;
	//		start = p;
	//	}
	//	if (*p)
	//	{
	//		break;
	//	}
	//}

	logM *roleList = listExistRole(soc);
	if (getNameOrRoleInput(roleList->userMessage, pUserMsg->role) == 1)
	{
		strcat(allMsg, pUserMsg->role);
		strcat(allMsg, ":");
	}

	char *pum;
	char password[100] = { 0 };
	while (1)
	{
		memset(pUserMsg->password, 0, sizeof(pUserMsg->password));
		memset(password, 0, sizeof(password));
		printf("\n------------------------------------------------------------------------\n");
		printf("\t请输入密码<1-8位数字、字母或它们的组合.>:");
		judgePasswd(password);
		pum = password;
		if (travUserMesg(pum) == 1)
		{
			printf("\n\t请再次确认密码:");
			judgePasswd(pUserMsg->password);
			pum = pUserMsg->password;
			if (travUserMesg(pum) == 1)
			{
				if (strcmp(password, pUserMsg->password) == 0)
				{
					strcat(allMsg, pUserMsg->password);
					break;
				}
				system("color 0C");
				printf("\n\n******************************** 错误 **********************************\n");
				printf("\t两次输入的密码不同！请检查重新输入！");
				enterReturnMenu();
				printf("\n\n------------------------------------------------------------------------\n");

			}
		}
		else
		{
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("\t密码输入格式不正确\n");
			enterReturnMenu();
			printf("\n\n------------------------------------------------------------------------\n");

		}
	}

	strcpy(plogMRe->userMessage, allMsg);
	plogMRe->headFlag[0] = 2;
	plogMRe->headFlag[1] = 3;

	send(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);			//发送数据
	//printf("\n发送用户名确认headFlag[1]:%d %s\n", plogMRe->headFlag[1], plogMRe->userMessage);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);		//接受服务器返回的数据
	//printf("received server :%d %s\n", plogMRe->headFlag[1], plogMRe->userMessage);

	if (3 == plogMRe->headFlag[1])
	{
		printf("\n\n------------------------------------------------------------------------\n");

		printf("\t修改成功！");
		enterReturnMenu();

	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\t修改失败！请检查您的操作！\n");
		printf("\n\t【%s】\n", plogMRe->userMessage);
		enterReturnMenu();
	}

	free(plogM);
	free(plogMRe);
	free(pUserMsg);
	free(allMsg);
}
void queryUser(SOCKET sockClient)
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//发送向服务其请求查询的消息
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//接受服务器回传的状态信息和用户信息
	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** 用户管理系统 ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t> ● 用户管理\n");
	printf("\t\t> ● 查询用户\n");
	printf("------------------------------------------------------------------------\n");
	logM *nameList = listExistUserName(soc);
	char queryName[20] = { 0 };
	//输入要查询的用户名，列出该用户的详细信息
	if (getNameOrRoleInput(nameList->userMessage, queryName) == 1)
	{
		strcpy(plogM->userMessage, queryName);
	}
	plogM->headFlag[0] = 2;
	plogM->headFlag[1] = 4;
	plogM->headFlag[2] = 2;
	send(sockClient, (char *)plogM, sizeof(*plogM), 0);	//发送数据
	//printf("\n发送用户名确认headFlag[2]:%d %s\n", plogM->headFlag[2], plogM->userMessage);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);	//接受服务器返回的数据
	//printf("received server :%d %s\n", plogMRe->headFlag[2], plogMRe->userMessage);

	strcat(plogMRe->userMessage, ":");
	if (2 == plogMRe->headFlag[2])
	{
		char *oneUserMsg = plogMRe->userMessage;
		char *pOum = oneUserMsg;
		char buf[40] = { 0 };

		printf("\n\n------------------------------------------------------------------------\n");
		printf("用户信息详细信息:\n");
		int i = 0;
		while (*oneUserMsg != '\0')
		{
			if (*oneUserMsg == ':')
			{
				strncpy(buf, pOum, oneUserMsg - pOum);
				buf[oneUserMsg - pOum] = '\0';
				userMsgTurnS(i++);
				printf("%s\n", buf);

				pOum = pOum + (oneUserMsg - pOum) + 1;
			}
			oneUserMsg++;
		}

		printf("\n\n------------------------------------------------------------------------\n");
		printf("\t查询用户结束\n");
	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\t查询用户失败\n");
		printf("\n\t【%s】\n", plogMRe->userMessage);
	}
	enterReturnMenu();
	free(plogM);
	free(plogMRe);
}
logM *listExistUserName(SOCKET sockClient)
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//发送向服务其请求查询的消息
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//接受服务器回传的状态信息和用户信息

	userMsg *pUserMsg = (userMsg *)calloc(1, sizeof(userMsg)); //接受输入的修改信息
	char *allMsg = (char *)calloc(1024, sizeof(char));			//保存修改信息，

	plogM->headFlag[0] = 2;		//用户管理标志
	plogM->headFlag[1] = 4;		//查询标志
	plogM->headFlag[2] = 1;		//申请查询标志
	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//发送请求查询数据
	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);		//接受服务器返回的数据判断是否可查
	if (1 == plogMRe->headFlag[2])
	{
		//列出已存在所有用户名
		char *allUserName = plogMRe->userMessage;
		char *pAum = allUserName;
		char buf[20] = { 0 };

		printf("\n\n------------------------------------------------------------------------\n");
		printf("所有用户信息:\n");
		while (*allUserName != '\0')
		{
			if (*allUserName == ':')
			{
				strncpy(buf, pAum, allUserName - pAum);
				buf[allUserName - pAum] = '\0';
				printf("\t用户名： %s\n", buf);
				pAum = pAum + (allUserName - pAum) + 1;
			}
			allUserName++;
		}

	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("请求出错，不能列出所有用户名！\n");
		enterReturnMenu();
	}
	free(plogM);
	return plogMRe;
}

////////////////////////////////角色管理////////////////////////////////////
void getPrivilege(char *buf, int i)
{
	char temp[10];
	while (1)
	{
		memset(temp, 0, sizeof(buf));
		printf("\t%s: ", permissionMessg[i]);

		fgets(temp, 5, stdin);
		if (strlen(temp) == 2 && temp[1] == '\n')
		{
			if (temp[0] == '1' || temp[0] == '0')
			{
				buf[i] = temp[0];
				break;
			}
		}
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("输入格式不正确！请重新输入\n");
		enterReturnMenu();
		printf("\n\n------------------------------------------------------------------------\n");
		fflush(stdin);
	}
}
void addRole(SOCKET sockClient)				//增加角色、
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//发送向服务其请求查询权限的消息
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//接受服务器回传的状态信息和权限信息

	char roleName[100] = { 0 };					//暂存角色名称
	char roleNameAndPermission[1024] = { 0 };   //暂存要发给服务器的信息


	if (!soc)
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\t网络错误！可能是由<socket>故障引起！");
		enterReturnMenu();
	}

	plogM->headFlag[0] = 3;
	plogM->headFlag[1] = 1;

	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** 用户管理系统 ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t> ● 角色管理\n");
	printf("\t\t> ● 增加角色\n");
	printf("------------------------------------------------------------------------\n");
	printf("\n\n------------------------------------------------------------------------\n");
	while (1)
	{
		printf("\n\n------------------------------------------------------------------------\n");
		printf("请输入角色名称（1-10位字母或数字）：");
		fgets(roleName, 20, stdin);
		if (strlen(roleName) <= 10 && strlen(roleName) > 0)
		{
			roleName[strlen(roleName) - 1] = '\0';
			if (travUserMesg(roleName) == 1)
			{
				break;
			}
		}
		else{
			system("color 0C");
			printf("\n\n******************************** 错误 **********************************\n");
			printf("输入格式不正确！请重新输入\n");
			enterReturnMenu();
		}

	}
	strcat(roleNameAndPermission, roleName);
	strcat(roleNameAndPermission, ":");
	fflush(stdin);
	printf("\n\n------------------------------------------------------------------------\n");
	printf("请选择要给所添加的角色赋予的权限：");
	printf("\n输入1表示赋予该权限，0表示不赋予该权限！\n");
	printf("------------------------------------------------------------------------\n");

	int i = 0;
	char buf[15] = { 0 };
	while (i < 10)
	{
		getPrivilege(buf, i);
		i++;
	}

	strcat(roleNameAndPermission, buf);

	strcpy(plogM->userMessage, roleNameAndPermission);
	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//发送请求查询数据
	//printf("\n增加角色确认发送：\n heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);
	//printf("收到服务器传回的字符串：\n heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);

	if (1 == plogMRe->headFlag[1])
	{
		printf("\n\n------------------------------------------------------------------------\n");
		printf("\t角色添加成功！");

	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\t角色添加失败！");
		printf("\n\t【%s】\n", plogMRe->userMessage);
	}
	enterReturnMenu();
	free(plogM);
	free(plogMRe);
}
void deleteRole(SOCKET sockClient)			//删除角色
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//发送向服务其请求查询权限的消息
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//接受服务器回传的状态信息和权限信息
	char roleName[100] = { 0 };

	if (!soc)
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\t网络错误！可能是由<socket>故障引起！");
		enterReturnMenu();
	}

	plogM->headFlag[0] = 3;
	plogM->headFlag[1] = 2;
	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** 用户管理系统 ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t> ● 角色管理\n");
	printf("\t\t> ● 删除角色\n");
	printf("------------------------------------------------------------------------\n");
	logM *roleList = listExistRole(soc);
	if (getNameOrRoleInput(roleList->userMessage, roleName) == 1)
	{
		strcat(plogM->userMessage, roleName);
	}


	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//发送请求查询数据
	//printf("\n删除角色确认：\n heafFlag[1]=%d   %s", plogMRe->headFlag[1], plogMRe->userMessage);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);
	//printf("\n服务器传回：\n heafFlag[1]=%d   %s", plogMRe->headFlag[1], plogMRe->userMessage);

	if (2 == plogMRe->headFlag[1])
	{
		printf("\n\n------------------------------------------------------------------------\n");
		printf("\t删除成功！\n");
	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\t删除失败！\n");
		printf("\n\t【%s】\n", plogMRe->userMessage);
	}
	enterReturnMenu();
}
void changeRole(SOCKET sockClient)			//修改角色
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//发送向服务其请求查询权限的消息
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//接受服务器回传的状态信息和权限信息

	char roleName[100] = { 0 };					//暂存角色名称
	char roleNameAndPermission[1024] = { 0 };   //暂存要发给服务器的信息
	char changeName[200] = { 0 };

	//输入要修改的角色名
	plogM->headFlag[0] = 3;
	plogM->headFlag[1] = 3;
	memset(plogMRe, 0, sizeof(logM));
	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** 用户管理系统 ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t> ● 角色管理\n");
	printf("\t\t> ● 修改角色\n");
	printf("------------------------------------------------------------------------\n");
	logM *roleList = listExistRole(soc);
	if (getNameOrRoleInput(roleList->userMessage, roleName) == 1)
	{
		strcat(roleNameAndPermission, roleName);
		strcat(roleNameAndPermission, ":");
	}

	fflush(stdin);

	printf("\n\n------------------------------------------------------------------------\n");
	printf("\n请重新赋予该角色权限。\n");
	printf("\n输入1表示赋予该权限，0表示不赋予该权限！\n");

	int i = 0;
	char buf[15] = { 0 };
	while (i < 10)
	{
		getPrivilege(buf, i);
		i++;
	}

	strcat(roleNameAndPermission, buf);

	strcpy(plogM->userMessage, roleNameAndPermission);
	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//发送请求查询数据
	//printf("\n增加角色确认发送：\n heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);

	//printf("收到服务器传回的字符串：\n heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);
	strcpy(userInfo, plogMRe->userMessage);
	if (3 == plogMRe->headFlag[1])
	{
		printf("\n\n------------------------------------------------------------------------\n");

		printf("\n角色修改成功！\n");
	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\n角色修改失败！\n");
		printf("\n\t【%s】\n", plogMRe->userMessage);
	}
	enterReturnMenu();
	free(plogM);
	free(plogMRe);
}
void queryRole(SOCKET sockClient)			//查询角色
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//发送向服务其请求查询权限的消息
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//接受服务器回传的状态信息和权限信息

	//输入要查询的角色名，列出该用户的详细信息
	char queryName[200] = { 0 };

	plogM->headFlag[0] = 3;
	plogM->headFlag[1] = 4;
	plogM->headFlag[2] = 2;
	memset(plogMRe, 0, sizeof(logM));

	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** 用户管理系统 ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t> ● 角色管理\n");
	printf("\t\t> ● 查询角色\n");
	printf("------------------------------------------------------------------------\n");

	logM *roleList = listExistRole(soc);
	if (getNameOrRoleInput(roleList->userMessage, queryName) == 1)
	{
		strcpy(plogM->userMessage, queryName);
	}

	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//发送数据
	//printf("\n发送用户名确认headFlag[2]:%d %s\n", plogM->headFlag[2], plogM->userMessage);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);		//接受服务器返回的数据
	//printf("received server :%d %s\n", plogMRe->headFlag[2], plogMRe->userMessage);

	strcat(plogMRe->userMessage, ":");
	if (2 == plogMRe->headFlag[2])
	{
		char *oneRolePermission = plogMRe->userMessage;
		printf("\n\n------------------------------------------------------------------------\n");
		printf("角色详细信息:\n");
		for (int i = 0; oneRolePermission[i] != ':'; i++)
		{
			if (oneRolePermission[i] == '1')
			{
				printf("\t%s\n", permissionMessg[i]);
			}
		}

	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\n查询失败！\n");
		printf("\n\t【%s】\n", plogMRe->userMessage);
	}
	enterReturnMenu();
	free(plogM);
	free(plogMRe);
}
logM *listExistRole(SOCKET sockClient)		//列出当前所有角色名
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//发送向服务其请求查询权限的消息
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//接受服务器回传的状态信息和权限信息

	if (!soc)
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\t网络错误！可能是由<socket>故障引起！");
		enterReturnMenu();
	}
	plogM->headFlag[0] = 3;
	plogM->headFlag[1] = 4;
	plogM->headFlag[2] = 1;

	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//发送请求查询角色
	//printf("\n增加角色送确认：\n heafFlag[1]=%d\n", plogMRe->headFlag[2]);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);
	//printf("\n服务器传回：\n heafFlag[1]=%d\n", plogMRe->headFlag[2]);

	if (1 == plogMRe->headFlag[2])	//请求查询角色成功
	{
		//列出已存在所有角色名称
		char *allRoleName = plogMRe->userMessage;
		char *pArn = allRoleName;
		char buf[20] = { 0 };		//暂存角色一条名称

		printf("\n\n------------------------------------------------------------------------\n");
		printf("所有角色信息:\n");
		while (*allRoleName != '\0')
		{
			if (*allRoleName == ':')
			{
				strncpy(buf, pArn, allRoleName - pArn);
				buf[allRoleName - pArn] = '\0';
				printf("\t角色名： %s\n", buf);
				//memset(buf, 0, sizeof(buf));
				pArn = pArn + (allRoleName - pArn) + 1;
			}
			allRoleName++;
		}
	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("\n请求查询角色信息失败！");
		printf("\n\t【%s】\n", plogMRe->userMessage);
	}

	free(plogM);
	//free(plogMRe);
	return plogMRe;
}

////////////////////////////////权限管理////////////////////////////////////
void changePermission(SOCKET sockClient)			//修改权限
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//发送向服务其请求查询权限的消息
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//接受服务器回传的状态信息和权限信息
	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** 用户管理系统 ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t> ● 权限管理\n");
	printf("\t\t> ● 修改权限\n");
	printf("------------------------------------------------------------------------\n");
	if (!soc)
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("网络错误！可能是由<socket>故障引起！");
		enterReturnMenu();
	}

	char permissionNum[20] = { 0 };

	printf("\n\n------------------------------------------------------------------------\n");
	//系统已有权限列表:
	queryPermission(soc, -1);
	printf("------------------------------------------------------------------------\n");

	int i = 0;
	printf("请重新确认当前系统权限:\n");
	while (i < 10)
	{
		getPrivilege(permissionNum, i);
		i++;
	}

	plogM->headFlag[0] = 4;	//权限管理标志
	plogM->headFlag[1] = 1;	//申请修改标志
	strcpy(plogM->userMessage, permissionNum);

	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//发送请求查询数据

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);
	//printf("收到服务器传回的字符串：\n heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);
	strcpy(userInfo, plogMRe->userMessage);
	if (1 == plogMRe->headFlag[1])
	{
		printf("\n\n------------------------------------------------------------------------\n");
		printf("修改成功！\n");
	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("修改失败！\n");
		printf("\n\t【%s】\n", plogMRe->userMessage);

	}
	enterReturnMenu();

	free(plogM);
	free(plogMRe);
}
void queryPermission(SOCKET sockClient, int flag)		//查询权限
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//发送向服务其请求查询权限的消息
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//接受服务器回传的状态信息和权限信息
	if (flag == 1)
	{
		printf("------------------------------------------------------------------------\n");
		printf("\t\t\t  **** 用户管理系统 ****\n");
		printf("------------------------------------------------------------------------\n");
		printf("\t> ● 权限管理\n");
		printf("\t\t> ● 查询权限\n");
		printf("------------------------------------------------------------------------\n");
	}

	if (!soc)
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("网络错误！可能是由<socket>故障引起！");
		enterReturnMenu();
	}
	plogM->headFlag[0] = 4;	//权限管理标志
	plogM->headFlag[1] = 2;	//申请查询标志

	send(sockClient, (char *)plogM, sizeof(*plogM), 0);		//发送请求查询数据

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);
	//printf("收到服务器传回的字符串：heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);

	if (2 == plogMRe->headFlag[1])
	{
		//列出已存在所有权限
		char *pAp = plogMRe->userMessage;  //是0 1字符串
		int len = strlen(pAp);

		printf("当前系统权限信息:\n");
		for (int i = 0; i < len; i++)
		{
			if (pAp[i] != '0')
			{
				printf("\t%s\n", permissionMessg[i]);
			}
		}
	}
	else
	{
		system("color 0C");
		printf("\n\n******************************** 错误 **********************************\n");
		printf("请求失败！\n");
		printf("\n\t【%s】\n", plogMRe->userMessage);
		enterReturnMenu();
	}

	free(plogM);
	free(plogMRe);

}
void listExistPermission()			//列出当前所有权限名
{
	for (int i = 0; i < 10; i++)
	{
		printf("\t%s\n", permissionMessg[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////
void judgePasswd(char *password)
{
	char c;
	int a = 0;
	for (int i = 0; i < 100; i++)
	{
		c = _getch();
		if (c == 13)
		{
			break;
		}
		if ((c <= 90 && c >= 65) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57))
		{
			printf("*");
			password[i] = c;
		}
		while (c == 8 && i > 0)
		{
			printf("\b \b");
			i--;
			c = _getch();
			a++;
		}
		while (a >= 1)
		{
			c = _getch();
			if ((c <= 90 && c >= 65) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57))
			{
				printf("*");
				password[i] = c;
				a--;
				if (a < 1)
				{
					i--;
				}
				i++;
			}
		}
	}
}
void userManage(SOCKET sockClient)
{
	SOCKET sok = sockClient;

	int chooseU = -1;
	while (chooseU)				//循环用户管理 的4个选项
	{
		userManageMenu();
		printf("\n\n------------------------------------------------------------------------\n");
		printf("\n请选择功能:");
		scanf("%d", &chooseU);
		system("cls");

		if (getchar() != '\n')  //排除1A  的输入
		{
			printf("\n\n******************************** 警 告 **********************************\n");
			printf("请重新输入下列选项编号：\n");
			continue;
		}

		if (chooseU == 1 && userInfo[0] == '1')			//增加用户
		{
			addUser(sok);
		}
		else if (2 == chooseU && userInfo[1] == '1')	//删除用户
		{
			deleteUser(sok);
		}
		else if (3 == chooseU && userInfo[2] == '1')	//修改用户
		{
			changeUser(sok);
		}
		else if (4 == chooseU && userInfo[3] == '1')	//查询用户
		{
			queryUser(sok);
		}
		else if (0 != chooseU)
		{
			printf("\n\n******************************** 警 告 **********************************\n");
			printf("您的输入有误！请重新输入：\n");

		}
	}
}
void roleManage(SOCKET sockClient)
{
	SOCKET sok = sockClient;
	int chooseR = -1;
	fflush(stdin);
	while (chooseR)
	{
		roleManageMenu();
		printf("\n\n------------------------------------------------------------------------\n");
		printf("请选择功能:");
		scanf("%d", &chooseR);
		system("cls");
		if (getchar() != '\n')			//排除1A  的输入
		{
			printf("\n\n******************************** 警 告 **********************************\n");
			printf("请重新输入下列选项编号：\n");
			continue;
		}

		if (chooseR == 1 && userInfo[4] == '1')		//增加角色
		{

			addRole(sok);
		}
		else if (2 == chooseR && userInfo[5] == '1')	//删除角色
		{
			deleteRole(sok);
		}
		else if (3 == chooseR && userInfo[6] == '1')	//修改角色
		{
			changeRole(sok);
		}
		else if (4 == chooseR && userInfo[7] == '1')	//查询角色
		{
			queryRole(sok);
		}
		else if (0 != chooseR)
		{

			printf("\n\n******************************** 错误 **********************************\n");
			printf("您的输入有误！请重新输入：\n");
		}
	}
}
void permissionManage(SOCKET sockClient)
{
	SOCKET sok = sockClient;
	int chooseP = -1;

	while (chooseP)
	{
		permissionManageMenu();
		fflush(stdin);
		printf("\n\n------------------------------------------------------------------------\n");
		printf("请选择功能:");
		scanf("%d", &chooseP);
		system("cls");
		if (getchar() != '\n')						//排除1A  的输入
		{
			printf("\n\n------------------------------------------------------------------------\n");
			printf("请重新输入下列选项编号：\n");
			continue;
		}
		if (chooseP == 1 && userInfo[8] == '1')		//修改权限
		{
			changePermission(sok);
		}

		else if (2 == chooseP && userInfo[9] == '1')	//查询权限
		{
			queryPermission(sok, 1);
			printf("\n\n------------------------------------------------------------------------\n");
			printf("查询权限结束\n");
			enterReturnMenu();
		}
		else if (0 != chooseP)
		{
			printf("\n\n******************************** 错误 **********************************\n");
			printf("您的输入有误！请重新输入：\n");
		}
	}

}
void getSystemTime(char *str)
{
	time_t nowtime;
	struct tm *timeinfo;
	time(&nowtime);
	timeinfo = localtime(&nowtime);
	int year, month, day;
	year = timeinfo->tm_year + 1900;
	month = timeinfo->tm_mon + 1;
	day = timeinfo->tm_mday;
	sprintf(str, "%d-%d-%d", year, month, day);
}
void userMsgTurnS(int num)
{
	char bufOneMsg[8][20] =
	{
		"用户名	:",
		"电  话	:",
		"邮  箱	:",
		"职  位	:",
		"创建者	:",
		"创建时间:",
		"角  色	:",
		"密  码	:"
	};
	printf("\t%s", bufOneMsg[num]);

}

int getNameOrRoleInput(char *list, char *msg)
{
	char *p, *start;
	char buf[20];
	printf("\n\n------------------------------------------------------------------------\n");
	printf("\t请输入上面列表中(用户/角色)名称:");
	while (1)
	{
		p = start = list;
		scanf("%s", msg);

		while (*p)
		{
			memset(buf, 0, sizeof(buf));
			p = strchr(start, ':');
			strncpy(buf, start, p - start);
			if (strcmp(buf, msg) == 0)
			{
				system("cls");
				return 1;
			}
			p++;
			start = p;
		}
		printf("------------------------------------------------------------------------\n");
		printf("\t请重新输入:");
	}
}