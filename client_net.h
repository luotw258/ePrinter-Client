#ifndef __CLIENT_NET_H_
#define __CLIENT_NET_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
using std::cout;
using std::endl;
using std::string;

class CClientNet
{
public:
	CClientNet();
	~CClientNet();
	CClientNet(const char *server_ip, unsigned short port);
    CClientNet(CClientNet &net);
public:
	int connect_to_server(void);
	int xrecv(char *buf, int len, \
	            int* schedule = NULL, int display_schedule = 0);
	int xsend(char *buf, int len, \
	            int* schedule = NULL, int display_schedule = 0);
	
private:
    char *server_ip;
	unsigned short port;					// 本地端口 
	int sockfd;
	struct sockaddr_in server_addr;
};


#endif // !__CLIENT_NET_H_

