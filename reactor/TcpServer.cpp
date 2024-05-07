#include "TcpServer.h"

TcpServer::TcpServer(const string &ip, unsigned short port, int timeout)
    : _acceptor(ip, port), _loop(_acceptor) {}
TcpServer::~TcpServer() {}
void TcpServer::start() {
    _acceptor.ready();
    _loop.loop();
}
void TcpServer::stop() { _loop.unloop(); }
void TcpServer::setAllCallback(TcpConnectionCallback &&onNewConnection,
                               TcpConnectionCallback &&onMessege,
                               TcpConnectionCallback &&onClose) {

    _loop.setEventCallback(std::move(onNewConnection),
                           TcpConnection::onEvent::onNewConnection);
    _loop.setEventCallback(std::move(onMessege),
                           TcpConnection::onEvent::onMessege);
    _loop.setEventCallback(std::move(onClose), TcpConnection::onEvent::onClose);
}
