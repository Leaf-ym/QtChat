/****************************************
*   ���ݿ���ز���						* 
*	����ģʽ��ȫ��ֻӵ��ʵ����			*
*	��˵ñ�֤���̰߳�ȫ�ġ�			*
*	created by lz 2017/5/25  12:32	    *
*****************************************/

#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <pthread.h>

//mysql ����ʵ��
MYSQL *mysql;

//mysql �����
MYSQL_RES *mysql_res;

//mysql ������е�һ��
MYSQL_ROW *mysql_row;


//��ʼ�����ݿ�����
int init_mysql();

//ִ��SQL
int execute_mysql(const char *sql);

//�ر�����
void close_mysql();

//������
void print_error_mysql(const char *msg);

/*********************���Ƿֽ���************************/
/******************Ӧ�ö����ݿ�Ĳ���*******************/

//�ж��û�id�������Ƿ�ƥ��
int login_check_mysql(const char *userid, const char *password);

//get friendlist with json
char *get_friendlist_json(const char *userid);


#endif // !DATABASE_H
