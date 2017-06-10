#include "recvfromclient.h"
#include "sendtoclient.h"
#include "Utility/utility.h"
#include "msgstructure.h"
#include "DataBase/database.h"
#include "error.h"
#include "forward.h"
#include "DataStructure/onlinehashtable.h"
#include <sys/epoll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

extern struct epoll_event ev;
extern int epoll_fd ;

void recvMsg(int fd)
{
    //��ȡ����һ�����ĳ���
    char *buf = (char *)malloc(sizeof(Request));
    readn(fd, buf, sizeof(Request));

    Request *r = (Request*)buf;
    ssize_t length = r->len;

    free(buf);
    buf = NULL;

    if (length < 0){
        //�ͻ����쳣����
        ev.data.fd = fd;
        ev.events = EPOLLIN|EPOLLET;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
        printf("client error occur \n");
        close(fd);
        return;
    }

    buf = (char *)malloc(sizeof(char) * length);
    int len = readn(fd, buf, length);
    if (len != length){
        ev.data.fd = fd;
        ev.events = EPOLLIN|EPOLLET;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
        printf("client error occur \n");
        close(fd);
        return;
    }
    Msg *msg = (Msg*)buf;

    switch (msg->type)
    {
    case REQUESTLOGIN:
        handleLoginMsg(fd, msg);
        break;
    case HEARTBEAT:
        //handleHeartBeatMsg(fd);
        break;
    case REQUESTFORWORDMESSAGE:
        //shandleForwordMessageMsg(fd, msg);
        break;
    default:
        break;
    }

    free(buf);
}


void handleLoginMsg(int fd, Msg *msg)
{
    LoginMsg lmsg;
    bool loginSuccess = false;
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
        loginSuccess = true;
        r_msg.ls = LOGINSUCCESS;
        //ά������״̬��Ҫ������

        //�������߶���
        OnlineUser user;
        user.fd = fd;
        strcpy(user.userid, lmsg.userid);
        addOnlineUser(&user);
        break;
    }

    sendResponseLoginMsg(fd, &r_msg);

    if (loginSuccess)
        sendResponseFriendList(fd, get_friendlist_json(lmsg.userid));

    printf("%s\n", get_friendlist_json(lmsg.userid));

    close_mysql();
}

void handleHeartBeatMsg(int fd)
{
    sendResponseHeartBeatMsg(fd);
}

void handleExitMsg(int fd)
{
    ev.data.fd = fd;
    ev.events = EPOLLIN|EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
    close(fd);
}

void handleForwordMessageMsg(int fd, Msg *msg)
{
    RequestForwordMessageMsg *fmsg = (RequestForwordMessageMsg*)malloc(msg->len * sizeof(char));
    memcpy(fmsg, msg->data, msg->len);


    char *message = (char*)malloc(fmsg->length * sizeof(char));

    strcpy(message, fmsg->message);

    int friend_fd = findOnlineUserWithUid(fmsg->friendid);

    printf("received userid %s\n", fmsg->friendid);
    printf("received %d\n", fmsg->length);

    for(int i = 0; i < fmsg->length; ++i)
    {
        printf("%c", message[i]);
    }

    if(friend_fd == -1)
    {
        //store message to database;
    }
    else if(friend_fd > 0)
    {
        //forward
        forwardmessage(fd, friend_fd, message);
    }


    free(message);
    free(fmsg);
}
