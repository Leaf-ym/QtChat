#ifndef SENDTOCLIENT_H
#define SENDTOCLIENT_H

#include "msgstructure.h"
#include <unistd.h>


//ͨ�÷��ͺ���
void sendMsg(int fd, MsgType msgtype, char *data, ssize_t size);

//������������Ӧ��
void sendResponseHeartBeatMsg(int fd);

//������ͨѶ
void sendResponseLoginMsg(int fd, ResponseLoginMsg *msg);

//send friendlist
void sendResponseFriendList(int fd, const char*);




//�����쳣ʱ��Ҫ���͵���Ϣ




#endif // !SENDTOCLIENT_H

