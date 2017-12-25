
#include "func.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void logMenu()
{
	system("color 0A");
	printf("\n\n");
	printf("\t************************* ��ӭ���� *************************\n");
	printf("\t*                                                          *\n");
	printf("\t*                     �� �� �� �� ϵ ͳ                    *\n");
	printf("\t*----------------------------------------------------------* \n");
	printf("\t*                                                          *\n");
	printf("\t*                            ----��ϵͳ��\"011����\"����---- *\n");
	printf("\t*                            --2016/05/20����2016/06/20--- *\n");
	printf("\t************************************************************\n");
	printf("\n");
	printf("------------------------------------------------------------------------");
}
logM *logToSystem()
{
	logM * plogM = (logM *)calloc(1, sizeof(logM)); //����ռ佫��¼���û���Ϣ����
	initStruct(plogM);								//��ʼ���ṹ��ָ��

	char userName[100] = { 0 };
	char password[100] = { 0 }; //������ʱ���������û���¼ʱ����Ϣ
	char allMessge[200] = { 0 };

	int userNameLen = 0;
	int passwordLen = 0;
	int count = 0;
	int icountu = 0;
	int icountp = 0;

	do
	{
		logMenu();
		printf("\n�����ȵ�¼:\n");
		printf("------------------------------------------------------------------------");
		printf("\n�������û���(��������ĸ�����ֻ��������,λ��<=8)��\n");
		fgets(userName, 21, stdin);  //��ȡ�����20λ
		userName[strlen(userName) - 1] = '\0';
		printf("------------------------------------------------------------------------");
		printf("\n����������(��������ĸ�����ֻ�������ϣ�λ��<=8)��\n");
		fflush(stdin);

		//�ж�����
		judgePasswd(password);

		fflush(stdin);

		userNameLen = strlen(userName);
		passwordLen = strlen(password);
		if (userNameLen > 8 || userNameLen <= 0)
		{
			count++;
			system("cls");
			system("color 0C");
			printf("\n��������û���λ������������ȷ��\n");
			enterReturnMenu();
			continue;

		}
		if (passwordLen > 8 || passwordLen <= 0)
		{
			count++;
			system("cls");
			system("color 0C");
			printf("\n�����������λ������������ȷ��\n");

			enterReturnMenu();
		}

		icountu = travUserMesg(userName);
		icountp = travUserMesg(password);

		if (count >= 3)
		{
			system("color 0C");
			printf("\a\a");
			printf("���Ѿ�������Σ�ϵͳ������3���Ӻ��˳���");
			Sleep(3000);
			exit(0);
		}
	} while (userNameLen > 8 || passwordLen > 8 || userNameLen <= 0 || passwordLen <= 0 || icountu != 1 || icountp != 1);


	strcat(allMessge, userName);
	strcat(allMessge, ":");
	strcat(allMessge, password);

	plogM->headFlag[0] = 1;
	memcpy(plogM->userMessage, allMessge, userNameLen + passwordLen + 1);

	return plogM;
}

void manageMenu()			//ϵͳ����˵�
{
	printf("------------------------------------------------------------------------\n");
	printf("\t\t**** �� �� �� �� ϵ ͳ ****\n");
	printf("------------------------------------------------------------------------\n");
	if (strncmp(userInfo, "0000", 4))
	{
		printf("\t\t1.�û�����\n");
	}
	if (strncmp(userInfo + 4, "0000", 4))
	{
		printf("\t\t2.��ɫ����\n");
	}
	if (strncmp(userInfo + 8, "00", 2))
	{
		printf("\t\t3.Ȩ�޹���\n");
	}
	printf("\n\t\t0.�� ��\n");
	printf("------------------------------------------------------------------------\n");

}
void userManageMenu()			//�û�����˵�
{
	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** �û�����ϵͳ ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t>�� �û�����\n");
	printf("------------------------------------------------------------------------\n");
	if (userInfo[0] == '1')
	{
		printf("\t\t1.�����û�.\n");
	}
	if (userInfo[1] == '1')
	{
		printf("\t\t2.ɾ���û�.\n");
	}
	if (userInfo[2] == '1')
	{
		printf("\t\t3.�޸��û�.\n");
	}
	if (userInfo[3] == '1')
	{
		printf("\t\t4.��ѯ�û�.\n");
	}
	printf("\n\t\t0.�� ��.\n");
	printf("------------------------------------------------------------------------\n");
}
void roleManageMenu()			//��ɫ����˵�
{
	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** �û�����ϵͳ ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t> �� ��ɫ����\n");
	printf("------------------------------------------------------------------------\n");
	if (userInfo[4] == '1')
	{
		printf("\t\t1.���ӽ�ɫ.\n");
	}
	if (userInfo[5] == '1')
	{
		printf("\t\t2.ɾ����ɫ.\n");
	}
	if (userInfo[6] == '1')
	{
		printf("\t\t3.�޸Ľ�ɫ.\n");
	}
	if (userInfo[7] == '1')
	{
		printf("\t\t4.��ѯ��ɫ.\n");
	}

	printf("\n\t\t0.�� ��.\n");
}
void permissionManageMenu()		//Ȩ�޹���˵�
{
	printf("------------------------------------------------------------------------\n");
	printf("\t\t\t  **** �û�����ϵͳ ****\n");
	printf("------------------------------------------------------------------------\n");
	printf("\t> �� Ȩ�޹���\n");
	printf("------------------------------------------------------------------------\n");
	if (userInfo[8] == '1')
	{
		printf("\t\t1.�޸�Ȩ��.\n");
	}
	if (userInfo[9] == '1')
	{
		printf("\t\t2.��ѯȨ��.\n");
	}
	printf("\n\t\t0.�� ��.\n");
}
void userAllMessageMenu()
{
	printf("1.�û���.\n");
	printf("2.�� ��.\n");
	printf("4.�� ��.\n");
	printf("5.ְλ.\n");
	printf("6.������.\n");
	printf("7.����ʱ��.\n");
	printf("8.��ɫ.\n");
	printf("0.�� ��.\n");
}

