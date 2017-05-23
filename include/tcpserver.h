#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define BUFSIZE 1024
namespace Game {
struct myEvent
{
   int fd;
   int epollfd;
   void (*call_back)(int fd, int events, void *arg);  
   int events;  
   void *arg;  
   int status; // 1: in epoll wait list, 0 not in  
   char buff[BUFSIZE]; // recv data buffer  
   int len, s_offset;  
   long last_active; // last active time  
};

class TcpServer {
public:
	TcpServer();
	~TcpServer();
	int Listen(int port);

private:
	// socket fd
	int listen_fd_;
    
    //epoll fd
    int epoll_fd_;

    //server address
	struct sockaddr_in serv_addr_;
};
}

#endif