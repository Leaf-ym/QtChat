/*
	�������Ļ����ܹ�
*/

#ifndef SERVER_H
#define SERVER_H

#include <poll.h>
#include <sys/epoll.h>
//���ش����ķ��������׽���
int init();


//int poll(struct pollfd fdarray[], nfds_t nfds, int timeout);
//�ɹ�����׼����������������������ʱ����0�� ������ - 1.
//��ʼ�������ӣ����ҽ���IO����
//void start(int fd, struct pollfd *fds, nfds_t *size);

void start(int fd);

#endif // !SERVER_H

