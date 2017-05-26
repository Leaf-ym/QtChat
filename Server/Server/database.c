#include "database.h"
#include "marco.h"

#include <stdio.h>
#include <string.h>

int init_mysql()
{
	mysql = mysql_init(NULL);

	//�������ݿ⡣���ʧ�ܣ�����-1.
	if (!mysql_real_connect(mysql, DATABASE_HOST, DATABASE_USER, DATABASE_PASSWORD,
		DATABASE_DBNAME, DATABASE_PORT, NULL, 0))
		return -1;

	return execute_mysql("set names utf8");
}

int execute_mysql(const char *sql)
{
	if (mysql_real_query(mysql, sql, strlen(sql)))
		return -1;

	return 0;
}

void close_mysql()
{
	mysql_close(mysql);
}

void print_error_mysql(const char *msg)
{
	if (msg)
		printf("%s : %s\n", msg, mysql_error(mysql));
	else
		puts(mysql_error(mysql));
}

/*
* ��ѯ�ж��û��Ƿ��ܹ�ƥ��
* ����ֵ����   ƥ��   �������   �Լ� �û�������
*/
int login_check_mysql(const char *userid, const char *password)
{
	char sql[DATABASE_SQLMAXLENGTH];
	sprintf(sql, "select userid, password from user where userid='%s';", userid);

	if (execute_mysql(sql) == -1)
		print_error_mysql(sql);

	//��ȡ��ѯ�Ľ����
	mysql_res = mysql_store_result(mysql);

	int r_count = mysql_num_rows(mysql_res);

	if (r_count < 1) {
		return DATABASE_USER_NOTEXIST;
	}

	//userid  ��Ψһ�ģ����ȷ���������ֻ��һ��
	mysql_row = mysql_fetch_row(mysql_res);


	if (strcmp(password, mysql_row[1]) == 0)
		return DATABASE_USER_CORRECT;
	else
		return DATABASE_USER_PASSWORDERROR;
}