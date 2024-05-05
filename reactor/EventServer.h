/**
 * @file EventServer.h
 * @brief 定义了reacotr服务器最上层的封装类
 * @details 同时也定义了事件驱动服务器中所需处理的事件类
 * @author  yanhao zhao
 * @date 2024/4/16
 * @version ver.1.0
 */
#ifndef __EVENT_SERVER__
#define __EVENT_SERVER__

#include "Task.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "ThreadPool.h"

/**
 * @brief 最外层的封装类
 * @details 封装了tcp服务器和线程池，并定义了3个事件回调函数作为成员
 */
class EventServer {
  public:
    /**
     * @brief Base constructor
     * @param thredNum 初始化线程池的线程数
     * @param queSize 初始化任务队列的队列大小
     * @param ip tcp服务器的ip
     * @param port tcp服务器的端口
     */
    EventServer(size_t thredNum, size_t queSize, const string &ip,
                unsigned short port, int timeout = 3000);

    /**
     * @brief Destructor
     */
    ~EventServer();

    /**
     * @brief EventServer的启动函数
     * @return void
     */
    void start();

    /**
     * @brief EventServer的停止函数
     * @return void
     */
    void stop();

    /**
     * @brief 服务器建立新的连接时调用的回调函数
     * @param &con 函数回调发生在传入的该连接上
     * @return void
     */
    void onNewConnection(const TcpConnectionPtr &con);

    /**
     * @brief 服务器处理就连接消息时的回调函数
     * @param &con 函数回调发生在传入的该连接上
     * @return void
     */
    void onHandleMessage(const TcpConnectionPtr &con);

    /**
     * @brief 服务器处理连接关闭时调用的回调函数
     * @param &con 函数回调发生在传入的该连接上
     * @return void
     */
    void onClose(const TcpConnectionPtr &con);

  private:
    ThreadPool _pool;  //!< 线程池成员
    TcpServer _server; //!< tcp服务器成员
};
#endif // __EVENT_SERVER__
