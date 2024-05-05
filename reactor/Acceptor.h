/**
 * Project Untitled
 */

#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "InetAddress.h"
#include "Socket.h"
#include <string>
using std::string;
class Acceptor {
  public:
    /**
     * @param ip
     * @param port
     */
    Acceptor(const string &ip, unsigned short port);
    ~Acceptor();

    void ready();

    int accept();

    int getFd() const;

  private:
    Socket _socket;
    InetAddress _addr;

    void setReuseAddr();

    void setReusePort();

    void bind();

    void listen();
};

#endif //_ACCEPTOR_H
