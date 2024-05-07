/**
 * @file EventServer.cc
 * @brief EventServer类成员函数和MyTask类成员函数的实现
 * @author  yanhao zhao
 * @date 2024/4/16
 * @version ver.1.0
 */

#include "EventServer.h"
#include <functional>

EventServer::EventServer(size_t threadNum, size_t queSize, const string &ip,
                         unsigned short port, int timeout)
    : _server(ip, port, timeout), _pool(threadNum, queSize) {}

/**
 * @details EventServer调用成员对象的启动函数，并将事件的回调函数进行初始化
 * @see ThreadPool::start
 * @see TcpServer::setAllCallback
 * @see TcpServer::start
 */
void EventServer::start() {
    _pool.start();
    using namespace std::placeholders;
    _server.setAllCallback(std::bind(&EventServer::onNewConnection, this, _1),
                           std::bind(&EventServer::onHandleMessage, this, _1),
                           std::bind(&EventServer::onClose, this, _1));
    _server.start();
}

/**
 * @details EventServer调用成员对象的停止函数
 * @see TcpServer::stop
 * @see ThreadPool::stop
 */
void EventServer::stop() { _server.stop(); }

EventServer::~EventServer() { _pool.stop(); }
/**
 * @details
 * 传给TcpConnection类的回调函数，当有客户端连接时会调用该函数，打印本机和客户端的信息
 * @see TcpConnection::infoAddrPort
 */
void EventServer::onNewConnection(const TcpConnectionPtr &con) {
    cout << con->infoAddrPort() << " has connected." << endl;
}

/**
 * @details
 * 传给TcpConnection类的回调函数，当有客户端发来消息时会调用该函数，将信息封装为任务，传给任务队列
 */
void EventServer::onHandleMessage(const TcpConnectionPtr &con) {
    string msg = con->receive();
    if (msg == "word\n") {
        string msg = con->receive();
        KeyWordTask keytask(msg, con);
        _pool.addTask(std::bind(&Mytask::process, keytask));
    } else if (msg == "web\n") {
        string msg = con->receive();
        WebTask webtask(msg, con);
        _pool.addTask(std::bind(&Mytask::process, webtask));
    } else {
        Mytask task(msg, con);
        _pool.addTask(std::bind(&Mytask::process, task));
    }
}

/**
 * @details
 * 传给TcpConnection类的回调函数，当有客户端断开连接时会调用该函数，打印本机和客户端的信息
 */
void EventServer::onClose(const TcpConnectionPtr &con) {
    cout << con->infoAddrPort() << " has closed." << endl;
}
