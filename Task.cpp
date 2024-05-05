#include "Task.h"
/**
 * @details
 * 该对象从onHandleMessage事件处获取客户端的消息及客户端与服务器端的连接用于初始化
 */
Mytask::Mytask(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg), _con(con) {}
