#ifndef __TCPSERVER__
#define __TCPSERVER__
#include "Acceptor.h"
#include "EventLoop.h"
#include <string>
using std::string;

class TcpServer {
  public:
    TcpServer(const string &, unsigned short port, int timeout);
    ~TcpServer();
    void start();
    void stop();
    void setAllCallback(TcpConnectionCallback &&, TcpConnectionCallback &&,
                        TcpConnectionCallback &&);

  private:
    Acceptor _acceptor;
    EventLoop _loop;
};

#endif
