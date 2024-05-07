/**
 * Project Untitled
 */

#include "SocketIO.h"
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
/**
 * SocketIO implementation
 */

/**
 * @param netfd
 */
SocketIO::SocketIO(int netfd) : _netfd(netfd) {}

SocketIO::~SocketIO() {}

/**
 * @param buf
 * @param size
 * @return int
 */
int SocketIO::readn(char *buf, int size) {
    int left = size;
    int ret = 0;
    char *backbuf = buf;
    // 可以恢复的错误，被系统调用打断，所以没有读取到数据
    while (left > 0) {
        ret = ::read(_netfd.getFd(), backbuf, left);
        if (-1 == ret && errno == EINTR) {
            continue;
        } else if (-1 == ret) {
            perror("read error");
            return -1;
        } else if (0 == ret) {
            break;
        } else {
            backbuf += ret;
            left -= ret;
        }
    }
    return size;
}

/**
 * @param buf
 * @param size
 * @return int
 */
int SocketIO::writen(const char *buf, int size) {
    int left = size;
    int ret = 0;
    const char *backbuf = buf;
    while (left > 0) {
        ret = ::write(_netfd.getFd(), backbuf, size);
        if (-1 == ret && errno == EINTR) {
            continue;
        } else if (-1 == ret) {
            perror("write error");
            return -1;
        } else if (0 == ret) {
            break;
        } else {
            left -= ret;
            backbuf += ret;
        }
    }
    return size;
}

/**
 * @param buf
 * @param size
 * @return int
 */
int SocketIO::readLine(char *buf, int size) {
    int left = size - 1;
    char *backbuf = buf;
    int ret = 0, total = 0;
    while (left > 0) {
        ret = recv(_netfd.getFd(), backbuf, left, MSG_PEEK);
        if (-1 == ret && errno == EINTR) {
            continue;
        } else if (-1 == ret) {
            perror("write error");
            return -1;
        } else if (0 == ret) {
            break;
        } else {
            for (int i = 0; i < ret; ++i) {
                if (backbuf[i] == '\n') {
                    int len = i + 1;
                    readn(backbuf, len);
                    backbuf += len;
                    *backbuf = '\0';
                    return total + len;
                }
            }

            readn(backbuf, ret);
            backbuf += ret;
            left -= ret;
            total += ret;
        }
    }
    *backbuf = '\0';
    return total;
}

int SocketIO::getFd() { return _netfd.getFd(); }
