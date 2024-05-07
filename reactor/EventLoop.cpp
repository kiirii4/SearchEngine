/**
 * Project Untitled
 */

#include "EventLoop.h"
#include "marco.h"
#include <iostream>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
/**
 * EventLoop implementation
 */

/**
 * @param acceptor
 */
EventLoop::EventLoop(Acceptor &acceptor, int timeout)
    : _acceptor(acceptor), _epfd(createEpollFd()), _isRunning(false), _mutex(),
      _eventList(1024), _onEventCallbacks(3), _evtfd(createEvtFd()),
      _timeout(timeout) {
    addEpollReadFd(_acceptor.getFd());
    addEpollReadFd(_evtfd);
}

EventLoop::~EventLoop() {
    ::close(_epfd);
    ::close(_evtfd);
}

/**
 * @return void
 */
void EventLoop::loop() {
    _isRunning = true;
    while (_isRunning) {
        waitEpollFd();
    }
}

/**
 * @return void
 */
void EventLoop::unloop() { _isRunning = false; }

/**
 * @return void
 */
void EventLoop::waitEpollFd() {
    int nready = 0;
    do {
        nready = epoll_wait(_epfd, &*_eventList.begin(), sizeof(_eventList),
                            _timeout);
    } while (-1 == nready && errno == EINTR);
    if (-1 == nready) {
        perror("epoll wait error");
        return;
    } else if (0 == nready) {
        std::cout << "timeout" << std::endl;
    } else {
        if (nready == static_cast<int>(_eventList.size())) {
            _eventList.reserve(nready * 2);
        }
        for (int idx = 0; idx < nready; ++idx) {
            int fd = _eventList[idx].data.fd;
            if ((fd == _acceptor.getFd()) && (fd & EPOLLIN)) {
                handleNewConnection();
            } else if ((fd == _evtfd) && (_eventList[idx].events & EPOLLIN)) {
                handleRead();
                doPendingFunctors();
            } else {
                if (_eventList[idx].events & EPOLLIN) {
                    handleMessage(fd);
                }
            }
        }
    }
}

/**
 * @return void
 */
void EventLoop::handleNewConnection() {
    int netfd = _acceptor.accept();
    if (netfd == -1) {
        perror("accept");
        return;
    }
    addEpollReadFd(netfd);

    std::shared_ptr<TcpConnection> con(new TcpConnection(netfd, this));

    // 遍历enum, 使用enum的值作为下标, 对回调函数数组进行赋值
    for (TcpConnection::onEvent evt = TcpConnection::onEvent::first;
         evt <= TcpConnection::onEvent::last;
         evt = static_cast<TcpConnection::onEvent>(static_cast<int>(evt) + 1)) {
        con->setEventCallback(_onEventCallbacks[static_cast<size_t>(evt)], evt);
    }
    _conns[netfd] = con;
    con->handleEventCallback(TcpConnection::onEvent::onNewConnection);
}

/**
 * @return void
 */
void EventLoop::handleMessage(int fd) {
    auto it = _conns.find(fd);
    if (it != _conns.end()) {
        bool flag = it->second->isClosed();
        // 若连接已关闭, 执行关闭的回调函数
        if (flag) {
            it->second->handleEventCallback(TcpConnection::onEvent::onClose);
            delEpollReadFd(fd);
            _conns.erase(it);
        }
        // 否则执行通信的回调函数
        else {
            it->second->handleEventCallback(TcpConnection::onEvent::onMessege);
        }
    } else {
        std::cout << "connection not exist" << std::endl;
    }
}

/**
 * @return int
 */
int EventLoop::createEpollFd() {
    int fd = ::epoll_create(100);
    if (fd < 0) {
        perror("epoll_create");
    }
    return fd;
}

int EventLoop::createEvtFd() {
    int fd = eventfd(10, 0);
    if (fd == -1) {
        perror("eventfd");
    }
    return fd;
}
/**
 * @param fd
 * @return void
 */
void EventLoop::addEpollReadFd(int fd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLERR;
    ev.data.fd = fd;
    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
    ERROR_CHECK(ret, 0, "epoll_ctl_add");
}

/**
 * @param fd
 * @return void
 */
void EventLoop::delEpollReadFd(int fd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLERR;
    ev.data.fd = fd;
    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &ev);
    ERROR_CHECK(ret, 0, "epoll_ctl_del");
}

void EventLoop::setEventCallback(TcpConnectionCallback &&cb,
                                 TcpConnection::onEvent evt) {
    _onEventCallbacks[static_cast<int>(evt)] = std::move(cb);
}

void EventLoop::runInLoop(functor &&func) {
    std::cout << "runInLoop" << std::endl;
    _mutex.lock();
    _pendings.push_back(std::move(func));
    _mutex.unlock();
    wakeup();
}

void EventLoop::doPendingFunctors() {
    vector<functor> temp;
    _mutex.lock();
    temp.swap(_pendings);
    _mutex.unlock();
    for (auto &cb : temp) {
        cb();
    }
}

void EventLoop::handleRead() {
    uint64_t num = 0;
    auto ret = read(_evtfd, &num, sizeof(num));
    if (sizeof(ret) != sizeof(uint64_t)) {
        perror("event read");
        return;
    }
}

void EventLoop::wakeup() {
    uint64_t num = 1;
    auto ret = write(_evtfd, &num, sizeof(num));
    if (sizeof(ret) != sizeof(uint64_t)) {
        perror("event write");
        return;
    }
}
