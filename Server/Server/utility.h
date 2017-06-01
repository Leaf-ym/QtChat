/*******************************************
*		    ���幫�õİ�������             *
*	      2017/5/24 10:35  by lz		   *
*******************************************/
#ifndef UTILITY_H
#define UTILITY_H

#include <unistd.h>

ssize_t readn(int filedes, void *buff, ssize_t nbytes);


ssize_t writen(int filedes, const void *buff, ssize_t nbytes);


ssize_t readline(int filedes, void *buff, ssize_t maxlen);


#endif // !UTILITY_H
