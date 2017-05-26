/****************************************
*   ��ϣ��								*
*	ά�������û�						*
*	��Ҫ��֤���̰߳�ȫ��				*
*	created by lz 2017/5/26  13:44	    *
*****************************************/

#ifndef ONLINEHASHTABLE_H
#define ONLINEHASHTABLE_H

#include "marco.h"
#include <stdbool.h>
//typedef struct _OnlineHashTable {
//	
//	//any??
//}OnlineHashTable;


typedef struct _OnlineUser {
	int fd;								//�����û���ӵ�е�socket������
	char userid[USERIDMAXLEN];			//�����û���ID�� ���ݴ�ID����ѯ�����Ƿ�����
}OnlineUser;

//�ڵ�
typedef struct _OnlineUserLinklist {
	OnlineUser user;
	struct _OnlineUserLinklist* next;
}OnlineUserLinklist;

OnlineUserLinklist *head;


void init_onlineuser();

//���һ�������û�
void addOnlineUser(OnlineUser *user);

//ɾ��һ�������û�
void delOnlineUser(const int fd);

//�����û�id ���������û��б�������߷���ture��
bool findOnlineUser(const char *userid);



#endif // !ONLINEHASHTABLE_H
