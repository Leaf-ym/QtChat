#ifndef MSGSTRUCTURE_H
#define MSGSTRUCTURE_H

#include "marco.h"

//��Ϣ����

typedef enum _MsgType {
	HEARTBEAT = 0,      //������
	REQUESTLOGIN        //�����½

}MsgType;

//Ϊ�˱���ճ������������ṹ������֪��������ݵĳ���
typedef struct _Resquest {
	long len;
}Request;


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

#endif // MSGSTRUCTURE_H
