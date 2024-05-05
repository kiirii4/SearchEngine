/**
 * Project Untitled
 */

#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>
using std::function;
using std::shared_ptr;
using std::string;
using std::vector;

class EventLoop;
class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
  public:
    /**
     * @param netfd
     */
    enum class onEvent {
        onNewConnection,
        onMessege,
        onClose,
        first = onNewConnection,
        last = onClose
    };

    explicit TcpConnection(int netfd, EventLoop *loop);

    ~TcpConnection();

    /**
     * @param msg
     */
    void send(const string &msg);
    string receive();

    string infoAddrPort();

    void sendInLoop(const string &msg);

    bool isClosed() const;

    // 设置回调函数
    void setEventCallback(const TcpConnectionCallback &cb, onEvent evt);
    // 执行回调函数
    void handleEventCallback(onEvent evt);
    // 将event的enum的值转为string
    string eventToString(onEvent evt);

  private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

  private:
    vector<TcpConnectionCallback> _onEventCallbacks;

    EventLoop *_loop;
    SocketIO _socketIO;
    Socket _sockfd;

    InetAddress _localAddr;
    InetAddress _peerAddr;
};

#endif //_TCPCONNECTION_H
