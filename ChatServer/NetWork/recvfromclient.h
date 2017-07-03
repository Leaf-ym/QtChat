#ifndef RECVFROMCLIENT_H
#define RECVFROMCLIENT_H
#include "msgstructure.h"


void recvConnectionMsg(int socketfd);

void recvMsg(int fd);



void handle(Msg *msg, int fd);

void handleLoginMsg(int fd, Msg *msg);

void handleHeartBeatMsg(int fd);

void handleExitMsg(int fd);

void handleForwordMessageMsg(int fd, Msg *msg);

void handleOfflineAckMessage(int fd);

void handleMoveFriendToGroup(int fd, RequestMoveFriendToGroup *msg);

void handleUpdateSignature(int fd, RequestUpdateSignature *msg);

void handleRequestGroupMessage(int fd);

void handleRequestGroupMemberMessage(int fd);

void handleRequestChangeStatus(int fd, Msg*msg);

void handleFrowardGroupMsg(int fd, Msg *msg);

void handleDeleteFriendMsg(int fd, Msg *msg);

void handleCreateGroupMsg(int fd, Msg*msg);

void handleSearchFriendMsg(int fd, Msg*msg);

void handleAddFriendMsg(int fd, Msg*msg);

void handleAddFriendAckMsg(int fd, Msg *msg);

void handleDeleteGroupMsg(int fd, Msg *msg);

void handleRenameFriendGroup(int fd, Msg *msg);
#endif // !RECVFROMCLIENT_H

