/**
 * Project Untitled
 */

#ifndef _SOCKET_H
#define _SOCKET_H

class Socket {
  public:
    Socket();

    /**
     * @param fd
     */
    explicit Socket(int fd);
    Socket(Socket &&);

    ~Socket();

    int getFd() const;

  private:
    int _fd;
};

#endif // _SOCKET_H
