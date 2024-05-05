/**
 * Project Untitled
 */

#ifndef _INETADDRESS_H
#define _INETADDRESS_H
#include <arpa/inet.h>
#include <cstdint>
#include <string>
using std::string;
class InetAddress {
  public:
    /**
     * @param port
     * @param ip
     */
    InetAddress(const string &ip, const unsigned short port);

    /**
     * @param addr
     */
    explicit InetAddress(struct sockaddr_in addr);

    InetAddress(InetAddress &&);

    string ip() const;

    unsigned short port() const;

    const struct sockaddr_in *getInetAddressPtr() const;

  private:
    struct sockaddr_in _addr;
};

#endif //_INETADDRESS_H
