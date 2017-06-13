#include "database.h"
#include "NetWork/marco.h"
#include "Utility/cJSON.h"

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


/*
    before  this serach we hava to ensure the userid is online and legitimate
    friendlist include:
        friendid, friendname, group, personalizedsignature.
        {
            group :
            [
                'friendid' : '123457', 'friendname':'liutimo', 'group':'friend', 'personalizedsignature':'i like code'
            ]
        }
*/
char *get_friendlist_json(const char *userid)
{

    cJSON *root = NULL;

    root = cJSON_CreateObject();

    char sql_getgroup[DATABASE_SQLMAXLENGTH], sql_getfriends[DATABASE_SQLMAXLENGTH];
    memset(sql_getgroup, 0, DATABASE_SQLMAXLENGTH);
    memset(sql_getfriends, 0, DATABASE_SQLMAXLENGTH);

    sprintf(sql_getgroup, "select distinct grouptype from friendlist where userid='%s';", userid);
    sprintf(sql_getfriends, "select friendid, username, remark, grouptype, personalizedsignature from friendlist, userinfo "
                          "where friendlist.userid='%s' and friendid = userinfo.userid;", userid);

    printf("%s\n", sql_getfriends);
    //get group
    if (execute_mysql(sql_getgroup) == -1)
        print_error_mysql(sql_getgroup);

    mysql_res = mysql_store_result(mysql);

    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
        cJSON_AddItemToObject(root, mysql_row[0], cJSON_CreateArray());
    }

    //get friends

    if (execute_mysql(sql_getfriends) == -1)
        print_error_mysql(sql_getfriends);

    mysql_res = mysql_store_result(mysql);

    printf("query count %d", mysql_affected_rows(mysql));

    //mysql_row[3] == frouptype
    while((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
        cJSON *node = root->child;
        while(node)
        {
            if(strcmp(node->string, mysql_row[3]) == 0)
            {
                cJSON *current = node;
                if(current == NULL)
                    printf("null\n");

                cJSON *tmp = cJSON_CreateObject();
                cJSON_AddStringToObject(tmp, "friendid", mysql_row[0]);
                cJSON_AddStringToObject(tmp, "username", mysql_row[1]);
                cJSON_AddStringToObject(tmp, "remark", mysql_row[2]);
                cJSON_AddStringToObject(tmp, "grouptype", mysql_row[3]);
                cJSON_AddStringToObject(tmp, "personalizedsignature", mysql_row[4]);
                cJSON_AddItemToArray(node, tmp);
            }
            node = node->next;
        }
    }

    return cJSON_PrintUnformatted(root);
}
