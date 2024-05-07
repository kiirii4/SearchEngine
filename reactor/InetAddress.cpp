/**
 * Project Untitled
 */

#include "InetAddress.h"
#include <cstring>
#include <string>
/**
 * InetAddress implementation
 */

/**
 * @param port
 * @param ip
 */
InetAddress::InetAddress(const string &ip, const unsigned short port) {
    ::memset(&_addr, 0, sizeof(sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(InetAddress &&addr) : _addr(std::move(addr._addr)) {
    ::memset(&(addr._addr), 0, sizeof(struct sockaddr_in));
}
/**
 * @param addr
 */
InetAddress::InetAddress(struct sockaddr_in addr) : _addr(addr) {}

/**
 * @return string
 */
string InetAddress::ip() const {
    return std::string(inet_ntoa(_addr.sin_addr));
}

/**
 * @return unsigned short
 */
unsigned short InetAddress::port() const { return ntohs(_addr.sin_port); }

/**
 * @return struct sockaddr_in *
 */
const struct sockaddr_in *InetAddress::getInetAddressPtr() const {
    return &_addr;
}
