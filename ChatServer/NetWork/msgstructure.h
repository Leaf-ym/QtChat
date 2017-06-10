#ifndef MSGSTRUCTURE_H
#define MSGSTRUCTURE_H

#include "marco.h"

//��Ϣ����
typedef enum _MsgType {
    HEARTBEAT = 0,			//������
    REQUESTLOGIN,			//�����½
    RESPONSELOGIN,			//��֪�ͻ��˵�¼״̬
    RESPONSEFRIENDLIST,     //�����б�
    EXIT,
    REQUESTFORWORDMESSAGE,  //���������ת����Ϣ
    RECEIVEDMESSAGE
}MsgType;


//������.  request��response��ʹ������ṹ
typedef struct _HeartBackMsg {
    char status;				//ȷ��Ϊ 'a';
}HeartBeatMsg;

//��¼״̬
typedef enum _LoginStatus {
    LOGINSUCCESS = 0,       //�ɹ���¼
    LOGINPWERROR,			//�������
    LOGINUNKNOW,		    //δ֪�û���¼
    LOGINREPEAT             //�ظ���½
}LoginStatus;

//Ϊ�˱���ճ������������ṹ������֪��������ݵĳ���
typedef struct _Resquest {
    long len;
}Request;

typedef struct _Response {
    long len;
}Response;


//ͨ����Ϣ�ṹ
typedef struct _Msg {
    MsgType type;   //��Ϣ����
    int len;        //��Ϣ����
    char data[0];   //��Ϣ
}Msg;


//��½��Ϣ
typedef struct _LoginMsg {
    char userid[USERIDMAXLEN];
    char password[PASSWORDMAXLEN];
}LoginMsg;

//��¼��Ӧ��Ϣ
typedef struct _ResponseLoginMsg {
    LoginStatus ls;				//��¼״̬
}ResponseLoginMsg;

//friendlist response
typedef struct _ResponseFriendList {
    int len;
    char friendlist[0];
}ResponseFriendList;

typedef struct _ExitMsg {

}ExitMsg;


typedef struct _RequestForwordMessageMsg {
    char friendid[USERIDMAXLEN];
    long length;
    char message[0];
}RequestForwordMessageMsg;

typedef struct _ReceivedMessageMsg {
    char friendid[USERIDMAXLEN];
    long length;
    char message[0];
}ReceivedMessageMsg;

#endif // MSGSTRUCTURE_H
