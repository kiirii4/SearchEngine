/**
 * Project Untitled
 */

#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H
#include "Acceptor.h"
#include "MutexLock.h"
#include "TcpConnection.h"
#include <functional>
#include <map>
#include <memory>
#include <vector>
using std::function;
using std::map;
using std::shared_ptr;
using std::vector;

using functor = function<void()>;
class EventLoop {
  public:
    /**
     * @param acceptor
     */
    explicit EventLoop(Acceptor &acceptor, int timeout = 3000);

    ~EventLoop();

    void loop();

    void unloop();

    void waitEpollFd();

    void handleNewConnection();

    void handleMessage(int fd);

    void setEventCallback(TcpConnectionCallback &&cb,
                          TcpConnection::onEvent evt);

    void runInLoop(functor &&);

    void wakeup();

  private:
    MutexLock _mutex;
    int _evtfd;
    int _epfd;
    int _timeout;
    vector<struct epoll_event> _eventList;
    bool _isRunning;
    Acceptor &_acceptor;
    map<int, shared_ptr<TcpConnection>> _conns;
    vector<TcpConnectionCallback> _onEventCallbacks;
    vector<functor> _pendings;
    TcpConnectionCallback _onTemp;

    int createEpollFd();
    int createEvtFd();
    /**
     * @param fd
     */
    void addEpollReadFd(int fd);

    /**
     * @param fd
     */
    void delEpollReadFd(int fd);

    void handleRead();

    void doPendingFunctors();
};

#endif //_EVENTLOOP_H
