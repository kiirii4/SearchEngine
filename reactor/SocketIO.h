/**
 * Project Untitled
 */

#ifndef _SOCKETIO_H
#define _SOCKETIO_H
#include "Socket.h"
class SocketIO {
  public:
    /**
     * @param netfd
     */
    explicit SocketIO(int netfd);

    ~SocketIO();

    /**
     * @param buf
     * @param size
     */
    int readn(char *buf, int size);

    /**
     * @param buf
     * @param size
     */
    int writen(const char *buf, int size);

    /**
     * @param buf
     * @param size
     */
    int readLine(char *buf, int size);

    int getFd();

  private:
    Socket _netfd;
};

#endif //_SOCKETIO_H
