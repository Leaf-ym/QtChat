#include "threadhandle.h"
#include "NetWork/recvfromclient.h"
#include "NetWork/marco.h"

#include <poll.h>
#include <unistd.h>
#include <stdio.h>



extern struct pollfd pollfds[LISTENMAXNUM];
extern nfds_t size;


void *pollhandle(void *arg)
{
	int ret, i;

    printf("entry poll thread"
           "\n");

	while (1)
	{
		ret = poll(pollfds, size, 0);
		if (ret > 0) {
			for (int i = 0; i < size; i++) {
				if (pollfds[i].revents & POLLIN) {
					//������Կ���ʹ���̳߳������
					recvMsg(pollfds[i].fd);
				}
			}
		}
	}
}
