#ifndef CLIENT_H
#define CLIENT_H
#include "tcpserver.h"
namespace Game {
struct Client {
    int playerid;
    int pos;
    myEvent *mev;
    Client(int id, myEvent * e) : playerid(id), mev(e) {}
};
}
#endif