/**
 * Project Untitled
 */

#include "Socket.h"
#include "marco.h"
#include <sys/socket.h>
#include <unistd.h>
/**
 * Socket implementation
 */

Socket::Socket() {
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(_fd, 0, "socket");
}

Socket::Socket(Socket &&socket) : _fd(socket._fd) { socket._fd = 0; }
/**
 * @param fd
 */
Socket::Socket(int fd) : _fd(fd) {}

Socket::~Socket() { ::close(_fd); }

/**
 * @return int
 */
int Socket::getFd() const { return _fd; }
