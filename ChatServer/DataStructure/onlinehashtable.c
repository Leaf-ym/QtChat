#include "onlinehashtable.h"
#include <stdio.h>
#include <string.h>
void init_onlineuser()
{
	head = (OnlineUserLinklist*)malloc(sizeof(OnlineUserLinklist));

	if (head == NULL)
	{
		printf("In onlinehashtable.c:�ڴ�ռ䲻���� ��ʼ�������û��б�ʧ��\n");
		exit(-1);
	}
}

//���һ�������û�
void addOnlineUser(OnlineUser *user)
{
	OnlineUserLinklist* node = (OnlineUserLinklist*)malloc(sizeof(OnlineUserLinklist));
	
	node->user.fd = user->fd;
	strcpy(node->user.userid, user->userid);

	node->next = head->next;
	head->next = node;
}

//ɾ��һ�������û�
void delOnlineUser(const int fd)
{
	OnlineUserLinklist* before = head;
	OnlineUserLinklist* current = head->next;
	while (current && current->user.fd != fd)
	{
		before = before->next;
		current = current->next;
	}

	if (current)
	{
		before->next = current->next;
		free(current);
	}
	else
		return;
}

//�����û�id ���������û��б�������߷���fd��
int findOnlineUser(const char *userid)
{
	OnlineUserLinklist* current = head->next;
	while (current && strcmp(current->user.userid, userid) != 0)
	{
		current = current->next;
	}

	if (current)
        return current->user.fd;
	else
        return -1s;
}
