#include <stdio.h>
#include <wait.h>
#include <signal.h>
#include <unistd.h>
#include <poll.h>
#include <pthread.h>
#include <string.h>

#include "msgstructure.h"
#include "database.h"
#include "error.h"
#include "server.h"
#include "marco.h"
#include "threadhandle.h"
#include "onlinehashtable.h"

//ȫ��
int fd;

struct pollfd pollfds[LISTENMAXNUM];
nfds_t size = 0;

void sig_handle(int signo)
{
	if (signo == SIGINT)
	{
		printf("received SIGINT signal\n");
		close(fd);
		exit(0);
	}
}

int main()
{

	pid_t pid;

	if (signal(SIGINT, sig_handle) == SIG_ERR)
		err_sys("signal error\n");


    printf("hello from Server!\n");
    
	//��ʼ�������û�����
	init_onlineuser();

	fd = init();

	pthread_create(&pid, NULL, pollhandle, NULL);
	
	start(fd, pollfds, &size);
	
	pthread_join(pid, NULL);

	return 0;
}