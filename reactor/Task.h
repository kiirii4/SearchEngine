#ifndef __TASK__
#define __TASK__

#include "../cache/CacheManager.h"
#include "../keyword_online/KeyRecommander.h"
#include "../reactor/Thread.h"
#include "../web_online/WebPageQuery.h"
#include "TcpConnection.h"
#include <string>

using std::string;
/**
 * @brief 用于封装reactor服务器框架处理的事件的类
 * @details
 * 该对象从onHandleMessage事件处获取客户端的消息及客户端与服务器端的连接用于初始化
 * ，并放入任务队列，具体业务由线程池的子线程执行
 */
class Mytask {
  public:
    /**
     * @brief Base constructor
     * @param msg 客户端传入的消息，任务需要对该信息进行处理
     * @param con
     * 线程池子线程在处理完任务后通知服务器，就是通过该连接指针将处理后的信息发送给客户端
     */
    Mytask(const string &msg, const TcpConnectionPtr &con);

    /**
     * @brief 具体执行业务的函数
     * @return void
     */
    virtual void process();

    /**
     * @brief Destructor
     */
    ~Mytask() = default;

  protected:
    string _msg; //!< 用于存储客户端发来的信息
    TcpConnectionPtr _con; //!< 用于存储发来信息的客户端和服务端之间的连接
};

/**
 * @details
 * 该对象从onHandleMessage事件处获取客户端的消息及客户端与服务器端的连接用于初始化
 */
inline Mytask::Mytask(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg), _con(con) {}

/**
 * @details
 * 调用TcpConnection类的sendInLoop函数，sendInLoop再调用tcp服务器中eventloop类的runInloop函数
 * @see EventLoop::runInloop
 */
inline void Mytask::process() {
    _msg += " is not valid.\n";
    _con->sendInLoop(_msg);
}

class WebTask : public Mytask {

  public:
    WebTask(const string &msg, const TcpConnectionPtr &con)
        : Mytask(msg, con) {}

    void process() override {
        CacheManager *cacheManager = CacheManager::getInstance();

        LRUCache cache = cacheManager->getCache(cache_idx);
        string cacheRes = cache.getRecord(_msg);

        if (!cacheRes.empty()) {
            _con->sendInLoop(cacheRes);
            return;
        }
        const string &result = WebPageQuery::doQuery(_msg);
        _con->sendInLoop(result);
    }
};

class KeyWordTask : public Mytask {
  public:
    KeyWordTask(const string &msg, const TcpConnectionPtr &con)
        : Mytask(msg, con) {}
    void process() override {
        CacheManager *cacheManager = CacheManager::getInstance();

        LRUCache cache = cacheManager->getCache(cache_idx);
        string cacheRes = cache.getRecord(_msg);

        if (!cacheRes.empty()) {
            std::cout << "cache hit" << "\n";
            _con->sendInLoop(cacheRes);
            return;
        }
        std::cout << "cache miss" << "\n";
        const string &result = KeyRecommander::doQuery(_msg);
        cache.addRecord(_msg, result);
        _con->sendInLoop(result);
    }
};

#endif
