/**
 * Project Untitled
 */

#include "Acceptor.h"
#include "Socket.h"
#include "marco.h"
/**
 * Acceptor implementation
 */

/**
 * @param ip
 * @param port
 */
Acceptor::Acceptor(const string &ip, unsigned short port)
    : _addr(ip, port), _socket() {}

Acceptor::~Acceptor() {}

/**
 * @return void
 */
void Acceptor::ready() {
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

/**
 * @return int
 */
int Acceptor::accept() {
    int netfd = ::accept(_socket.getFd(), nullptr, nullptr);
    if (netfd == -1) {
        perror("accept");
        return -1;
    }
    return netfd;
}

/**
 * @return void
 */
void Acceptor::setReuseAddr() {
    int opt = 1;
    int ret = setsockopt(_socket.getFd(), SOL_SOCKET, SO_REUSEADDR, &opt,
                         sizeof(opt));
    ERROR_CHECK(ret, 0, "setsockopt");
}

/**
 * @return void
 */
void Acceptor::setReusePort() {
    int opt = 1;
    int ret = setsockopt(_socket.getFd(), SOL_SOCKET, SO_REUSEPORT, &opt,
                         sizeof(opt));
    ERROR_CHECK(ret, 0, "setsockopt");
}

/**
 * @return void
 */
void Acceptor::bind() {
    int ret =
        ::bind(_socket.getFd(), (struct sockaddr *)_addr.getInetAddressPtr(),
               sizeof(struct sockaddr));
    ERROR_CHECK(ret, 0, "bind");
}

/**
 * @return void
 */
void Acceptor::listen() {
    int ret = ::listen(_socket.getFd(), 128);
    ERROR_CHECK(ret, 0, "listen");
}

int Acceptor::getFd() const { return _socket.getFd(); }
