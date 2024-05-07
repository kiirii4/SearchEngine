/**
 * Project Untitled
 */

#include "TcpConnection.h"
#include "EventLoop.h"
#include <iostream>
#include <sstream>
/**
 * TcpConnection implementation
 */

string TcpConnection::eventToString(TcpConnection::onEvent evt) {
    switch (evt) {
    case TcpConnection::onEvent::onNewConnection:
        return "NewConnection";
    case TcpConnection::onEvent::onMessege:
        return "Messege";
    case TcpConnection::onEvent::onClose:
        return "close";
    default:
        return "Unknown";
    }
}

bool TcpConnection::isClosed() const {
    char buf[10] = {0};
    int ret = ::recv(_sockfd.getFd(), buf, sizeof(buf), MSG_PEEK);
    return (0 == ret);
}
/**
 * @param netfd
 */
TcpConnection::TcpConnection(int netfd, EventLoop *loop)
    : _socketIO(netfd), _sockfd(netfd), _loop(loop), _localAddr(getLocalAddr()),
      _peerAddr(getPeerAddr()), _onEventCallbacks(3) {}

TcpConnection::~TcpConnection() {}

InetAddress TcpConnection::getLocalAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = getsockname(_sockfd.getFd(), (struct sockaddr *)&addr, &len);
    if (ret != 0) {
        perror("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = getpeername(_sockfd.getFd(), (struct sockaddr *)&addr, &len);
    if (ret != 0) {
        perror("getpeername");
    }
    return InetAddress(addr);
}
/**
 * @param msg
 * @return void
 */
void TcpConnection::send(const string &msg) {
    _socketIO.writen(msg.c_str(), msg.size());
}

/**
 * @return string
 */
string TcpConnection::receive() {
    char buf[65536] = {0};
    _socketIO.readLine(buf, sizeof(buf));
    return string(buf);
}

string TcpConnection::infoAddrPort() {
    std::ostringstream oss;
    oss << _localAddr.ip() << ": " << _localAddr.port() << " ----------> "
        << _peerAddr.ip() << " : " << _peerAddr.port();
    return oss.str();
}

void TcpConnection::setEventCallback(const TcpConnectionCallback &cb,
                                     onEvent evt) {
    _onEventCallbacks[static_cast<size_t>(evt)] = cb;
}

void TcpConnection::handleEventCallback(onEvent evt) {
    if (_onEventCallbacks[static_cast<size_t>(evt)]) {
        _onEventCallbacks[static_cast<size_t>(evt)](shared_from_this());
    } else {
        std::cout << eventToString(evt) << "cb == nullptr" << std::endl;
    }
}

void TcpConnection::sendInLoop(const string &msg) {
    if (_loop) {
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}
