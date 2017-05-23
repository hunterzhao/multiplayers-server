#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h>  
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <string.h>
#include <vector>
#include <string>

#include "tcpserver.h"
#include "parsemessage.h"
#include "client.h"
#include "cmd.h"
#define WAITCONN 100
#define MAX_EVENTS 500

namespace Game {
myEvent gEvents[MAX_EVENTS+1];
std::vector<Client> clients_;
int clientNum = 0;
void SendData(int fd, int events, void *arg);
void RecvData(int fd, int events, void *arg);  

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// set event  
void EventSet(myEvent *ev, int fd, void (*call_back)(int, int, void*), void *arg)  
{  
    ev->fd = fd;  
    ev->call_back = call_back;  
    ev->events = 0;  
    ev->arg = arg;  
    ev->status = 0;
    bzero(ev->buff, sizeof(ev->buff));
    ev->s_offset = 0;  
    ev->len = 0;
    ev->last_active = time(NULL);  
}  

// add/mod an event to epoll  
void EventAdd(int epollFd, int events, myEvent *ev)  
{  
    struct epoll_event epv = {0, {0}};  
    int op;  
    epv.data.ptr = ev;  
    epv.events = ev->events = events;  
    if(ev->status == 1){  
        op = EPOLL_CTL_MOD;  
    }  
    else{  
        op = EPOLL_CTL_ADD;  
        ev->status = 1;  
    }  
    if(epoll_ctl(epollFd, op, ev->fd, &epv) < 0) { 
        printf("Event Add failed[fd=%d], evnets[%d], epollFd[%d]\n", ev->fd, events, epollFd);  
        perror("ctl error");
    } else  
        printf("Event Add OK[fd=%d], op=%d, evnets[%0X]\n", ev->fd, op, events);  
}  

// delete an event from epoll  
void EventDel(int epollFd, myEvent *ev)  
{  
    struct epoll_event epv = {0, {0}};  
    if(ev->status != 1) return;  
    epv.data.ptr = ev;  
    ev->status = 0;
    epoll_ctl(epollFd, EPOLL_CTL_DEL, ev->fd, &epv);  
}  

// receive data  
void RecvData(int fd, int events, void *arg)  
{  
    struct myEvent *ev = (struct myEvent*)arg; 
    int len;  
    // receive data
    len = recv(fd, ev->buff, sizeof(ev->buff), 0);    
    EventDel(ev->epollfd, ev);
    if(len > 0)
    {
        ev->len = len;
        ev->buff[len] = '\0';  
        std::string str(ev->buff, ev->len);
        printf("C[%d]:%s\n", fd, str.c_str()); 

        std::vector<Cmd> con = str2Json(ev->buff, ev->len);
        // change to send event 
        printf("i will send to %d clients\n", clients_.size());
        
        for(size_t t = 0; t < con.size(); t++) {
            Cmd c = con[t];
            if(c.type == 0) {
              //存储到client中
              Client client(c.id, ev);
              clients_.push_back(client);
              clientNum++;
              //创建clientNum - 1个创建命令 发送给该client
              std::vector<Cmd> cmdCon;
              for (int i = 0 ; i < clientNum - 1; i++) {
                 if (c.id == clients_[i].playerid) 
                     cmdCon.push_back(con[t]);
                 Cmd t(c.id, 0.0, 0.0, 0);
                 cmdCon.push_back(t);
              }
              std::string str = json2Str(cmdCon);
              ev->len = str.size();
              ev->call_back = SendData;
              memmove(ev->buff, str.c_str(), ev->len);
              EventAdd(ev->epollfd, EPOLLOUT, ev);  
            } else {
              for (Client c : clients_) {
                //广播命令
                myEvent* e = c.mev;
                e->call_back = SendData;
                e->len = ev->len;
                memmove(e->buff, str.c_str(), len);
                EventAdd(e->epollfd, EPOLLOUT, e);  
              } 
            }
        }
        
        
    }  
    else if(len == 0)  
    {  
        close(ev->fd);  
        printf("[fd=%d] pos[%d], closed gracefully.\n", fd, events);  
    }  
    else  
    {  
        close(ev->fd);  
        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));  
    }  
}  

// send data  
void SendData(int fd, int events, void *arg)  
{  
    struct myEvent *ev = (struct myEvent*)arg;  
    int len;  
    // send data  
    len = send(fd, ev->buff, ev->len, 0);
    if(len > 0)  
    {
        printf("send[fd=%d], [%d<->%d]%s\n", fd, len, ev->len, ev->buff);
     
        // change to receive event
        EventDel(ev->epollfd, ev);  
        EventSet(ev, fd, RecvData, ev);  
        EventAdd(ev->epollfd, EPOLLIN, ev);  
    }  
    else  
    {  
        close(ev->fd);  
        EventDel(ev->epollfd, ev);  
        printf("send[fd=%d] error[%d]\n", fd, errno);  
        perror("send error");
    }  
}  

void AcceptConn(int fd, int events, void *args)
{
    struct sockaddr_in sin;
    socklen_t len = sizeof(struct sockaddr_in);  
    int nfd, i;  
    // accept  
    if((nfd = accept(fd, (struct sockaddr*)&sin, &len)) == -1)  
    {  
        if(errno != EAGAIN && errno != EINTR)  
        {  
        }
        printf("%s: accept, %d", __func__, errno);  
        return;  
    }
    int flag = 1;
    int ret = setsockopt(nfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
    if (ret == -1) {
      printf("Couldn't setsockopt(TCP_NODELAY)\n");
      exit(-1);
    }  

    do  
    {  
        for(i = 0; i < MAX_EVENTS; i++)  
        {  
            if(gEvents[i].status == 0)  
            {  
                break;  
            }  
        }  
        if(i == MAX_EVENTS)  
        {  
            printf("%s:max connection limit[%d].", __func__, MAX_EVENTS);  
            break;  
        }  
        // set nonblocking
        int iret = 0;
        if((iret = fcntl(nfd, F_SETFL, O_NONBLOCK)) < 0)
        {
            printf("%s: fcntl nonblocking failed:%d", __func__, iret);
            break;
        }

        // add a read event for receive data  
        EventSet(&gEvents[i], nfd, RecvData, &gEvents[i]);  
        EventAdd(gEvents[i].epollfd, EPOLLIN, &gEvents[i]);  

        
    }while(0);  
    printf("new conn[%s:%d][time:%d], pos[%d]\n", inet_ntoa(sin.sin_addr),
            ntohs(sin.sin_port), gEvents[i].last_active, i);  
}

TcpServer::TcpServer()
{
   epoll_fd_ = epoll_create(MAX_EVENTS);  
   if(epoll_fd_ <= 0) printf("create epoll failed.%d\n", epoll_fd_);  
   for(int i = 0; i < MAX_EVENTS; i++)  
   {  
      gEvents[i].epollfd = epoll_fd_;
   }  

   listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
   if (listen_fd_ < 0) 
        error("ERROR opening socket");

   // set non-blocking
   fcntl(listen_fd_, F_SETFL, O_NONBLOCK);   
   
   EventSet(&gEvents[MAX_EVENTS], listen_fd_, AcceptConn, &gEvents[MAX_EVENTS]);  
   
   // add listen socket  
   EventAdd(epoll_fd_, EPOLLIN, &gEvents[MAX_EVENTS]);  

   bzero((char *) &serv_addr_, sizeof(serv_addr_));
   serv_addr_.sin_family = AF_INET;
}

TcpServer::~TcpServer()
{

}

int TcpServer::Listen(int port)
{
   serv_addr_.sin_family = AF_INET;
   serv_addr_.sin_addr.s_addr = INADDR_ANY;
   serv_addr_.sin_port = htons(port);
   if (bind(listen_fd_, (struct sockaddr *) &serv_addr_, sizeof(serv_addr_)) < 0) 
              error("ERROR on binding");

   listen(listen_fd_, WAITCONN);
   std::cout << "server is the listening at " << port << std::endl;
   
   // event loop  
   struct epoll_event events[MAX_EVENTS];   
   int checkPos = 0;  
   
   while(1) {
      long now = time(NULL);
      // a simple timeout check here, every time 100, better to use a mini-heap, and add timer event  
      for (int i = 0 ; i < 100; i++, checkPos++)
      {
          if(checkPos == MAX_EVENTS) checkPos = 0; // recycle  
          if(gEvents[checkPos].status != 1) continue;  
          long duration = now - gEvents[checkPos].last_active;  
          if(duration >= 60) // 60s timeout  
          {  
              close(gEvents[checkPos].fd);  
              printf("[fd=%d] timeout[%d--%d].\n", gEvents[checkPos].fd, gEvents[checkPos].last_active, now);  
              EventDel(epoll_fd_, &gEvents[checkPos]);  
          }  
      }

      // wait for events to happen  
      int fds = epoll_wait(epoll_fd_, events, MAX_EVENTS, 1000);  
      if(fds < 0){  
          printf("epoll_wait error, exit\n");  
          break;  
      }  
      for(int i = 0; i < fds; i++){  
          myEvent *ev = (struct myEvent*)events[i].data.ptr;  
          if((events[i].events & EPOLLIN) && (ev->events & EPOLLIN)) // read event  
          {  
              ev->call_back(ev->fd, events[i].events, ev->arg);  
          }  
          if((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT)) // write event  
          {  
              ev->call_back(ev->fd, events[i].events, ev->arg);  
          }  
      }  
   }

   // int connsockfd = accept(listen_fd_, nullptr, nullptr);
   // if (connsockfd < 0) 
   //        error("ERROR on accept");

   // char buffer[256];
   // while(1) {
   //   bzero(buffer,256);
   //   int ret = read(connsockfd,buffer,255);
   //   if (ret < 0) 
   //   	  error("ERROR reading from socket");

   //   std::cout << "Here is the message: " << buffer << std::endl;
   //   ret = write(connsockfd, buffer, strlen(buffer));
   //   if (ret < 0) 
   //   	  error("ERROR writing to socket");
   // }
   // close(connsockfd);
   // close(listen_fd_);
}
}