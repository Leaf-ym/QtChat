#include "connecttoserver.h"
#include "marco.h"

#include <QDebug>
#include <QThread>
ConnectToServer::ConnectToServer(QObject *parent) : QTcpSocket(parent)
{
    connectToHost(IP, PORT);

    connect(this, &ConnectToServer::readyRead, this, &ConnectToServer::recv);
    //    connect(this, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
    //          [=](QAbstractSocket::SocketError socketError){ qDebug() << socketError; });
}

ConnectToServer* ConnectToServer::server = NULL;
QMutex* ConnectToServer::mutex = new QMutex();

//??????
ConnectToServer* ConnectToServer::getInstance()
{
    mutex->lock();

    if(server == NULL || !server->isValid())
        server = new ConnectToServer();

    mutex->unlock();

    return server;
}

void ConnectToServer::closeServer()
{
    this->close();
    server = NULL;
}


void ConnectToServer::send(MsgType msgtype, char *data, ssize_t size)
{
    mutex->lock();
    int memsize = (sizeof(Msg) + size) *sizeof(char);
    char *buf = new char[memsize];
    Msg *msg = (Msg*)buf;

    memset(msg, 0, memsize);

    msg->type = msgtype;
    msg->len = size;

    memcpy(msg->data, data, msg->len);

    Request r;
    r.len = memsize;


    if( -1 == write((char *)&r, sizeof(r))) {
        qDebug() << "??????????";
        delete []buf;
        return;
    }

    if (-1 == write((char *)msg, r.len))
        qDebug() << "??????????";

    delete []buf;

    mutex->unlock();
}

void ConnectToServer::sendLoginMsg(LoginMsg *loginmsg)
{
    send(REQUESTLOGIN, (char*)loginmsg, sizeof(LoginMsg));
}

void ConnectToServer::sendHeartBeatMsg(HeartBeatMsg *hearteabtmsg)
{
    send(HEARTBEAT, (char*)hearteabtmsg, sizeof(HeartBeatMsg));
}

void ConnectToServer::sendRequestForwordMessageMsg(RequestForwordMessageMsg *msg)
{
    send(REQUESTFORWORDMESSAGE, (char *)msg, sizeof(RequestForwordMessageMsg) + msg->length);
}

void ConnectToServer::sendRequestUserInfoMsg(RequestUserInfoMsg *msg)
{
    send(REQUESTUSERINFO, (char *)msg, sizeof(RequestUserInfoMsg));
}

void ConnectToServer::sendRequestOfflineMessage()
{
    RequestOfflineMessage *msg = new  RequestOfflineMessage;

    send(REQUESTOFFLINEMESSAGE, (char*)msg, sizeof(RequestOfflineMessage));

    delete msg;
}

/*****************************???????????????**************************************/

void ConnectToServer::recv()
{
    QByteArray bytearray = this->readAll();
    Msg *msg = (Msg*)bytearray.data();

    switch (msg->type) {
    case RESPONSELOGIN: {
        ResponseLoginMsg *rlm = new ResponseLoginMsg;
        memcpy(rlm, msg->data, sizeof(ResponseLoginMsg));
        emit loginStatus(rlm->ls);
        delete rlm;
        break;
    }
    case HEARTBEAT: {
        emit responseHeartBeat();
        break;
    }
    case RESPONSEFRIENDLIST: {
        char *buf = new char[msg->len];
        ResponseFriendList *rf = (ResponseFriendList*)buf;
        memcpy(rf, msg->data, msg->len);
        qDebug() << rf->friendlist;
        emit responseFriendList(QByteArray(rf->friendlist));
        delete []buf;
        break;
    }
    case RESPONSEUSERINFO: {
        ResponseUserinfo *rui = (ResponseUserinfo*)new char[msg->len];
        memcpy(rui, msg->data, msg->len);
        emit responseUserInfo(QByteArray(rui->userinfo));
        delete rui;
        break;
    }
    case RECEIVEDMESSAGE: {
        ReceivedMessageMsg *rmsg = (ReceivedMessageMsg*)new char[msg->len];
        memcpy(rmsg, msg->data, msg->len);
//        char  *message = new char[rmsg->length + 1];
//        memcpy(message, rmsg->message, rmsg->length);
//        message[rmsg->length] = '\0';
//        emit receivedMessage(message, rmsg->color, rmsg->size, rmsg->font);
        emit receivedMessage(rmsg);
        break;
    }
    case RESPONSEOFFLINEMESSAGE: {
        ResponseOfflineMessage *rom = (ResponseOfflineMessage*)new char[msg->len];
        memcpy(rom, msg->data, msg->len);
        emit receivedOfflineMessage(QByteArray(rom->json));
    }
    default:
        break;
    }
}
