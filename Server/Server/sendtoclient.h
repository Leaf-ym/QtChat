#ifndef SENDTOCLIENT_H
#define SENDTOCLIENT_H

#include "msgstructure.h"
#include <unistd.h>


//ͨ�÷��ͺ���

void sendMsg(int fd, Msg *msg, ssize_t msgsize);


//������ͨѶ

void sendResponseLoginMsg(int fd, ResponseLoginMsg *msg);





//�����쳣ʱ��Ҫ���͵���Ϣ




#endif // !SENDTOCLIENT_H

