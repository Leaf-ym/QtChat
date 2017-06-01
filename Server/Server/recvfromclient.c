#include "recvfromclient.h"
#include "sendtoclient.h"
#include "utility.h"
#include "msgstructure.h"
#include "database.h"
#include "error.h"
#include "onlinehashtable.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void recvMsg(int fd)
{
	
	//��ȡ����һ�����ĳ���
	char *buf = (char *)malloc(sizeof(Request));
	readn(fd, buf, sizeof(Request));
	Request *r = (Request*)buf;
	ssize_t length = r->len;
	free(buf);
	buf = NULL;

	buf = (char *)malloc(sizeof(char) * length);

	if (readn(fd, buf, length) != length)
		err_sys("��ȡ��Ϣʧ��");
	
	Msg *msg = (Msg*)buf;

	switch (msg->type)
	{
	case REQUESTLOGIN:
		handleLoginMsg(fd, msg);
		break;
	case HEARTBEAT:
		handleHeartBeatMsg(fd);
	default:
		break;
	}

	free(buf);
}


void handleLoginMsg(int fd, Msg *msg)
{
	LoginMsg lmsg;
	memcpy(&lmsg, msg->data, msg->len);

	init_mysql();
	
	ResponseLoginMsg r_msg;

	switch (login_check_mysql(lmsg.userid, lmsg.password))
	{
	case DATABASE_USER_PASSWORDERROR:
		r_msg.ls = LOGINPWERROR;
		break;
	case DATABASE_USER_NOTEXIST:
		r_msg.ls = LOGINUNKNOW;
		break;
	case DATABASE_USER_CORRECT:
		r_msg.ls = LOGINSUCCESS;
		//ά������״̬��Ҫ������

		//�������߶���
		OnlineUser user;
		user.fd = fd;
		strcpy(user.userid, lmsg.userid);
		addOnlineUser(&user);
		break;
	}
	close_mysql();
	
	sendResponseLoginMsg(fd, &r_msg);
}

void handleHeartBeatMsg(int fd)
{
	sendResponseHeartBeatMsg(fd);
}