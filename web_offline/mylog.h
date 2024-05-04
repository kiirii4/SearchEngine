/** @brief
 * 这是一个对log4cpp的封装类，旨在将日志系统中的所用到的类对象封装到一起，免去对日志对象的设置
 *  @file    mylog.h
 *  @author  赵彦浩
 *  @version 0.1
 *  @date    2024.03.12
 *  @note    使用本程序需要在编译时加上"-llog4cpp", "-lpthread"
 *  @since   2024
 */
#pragma once
#include "log4cpp/Appender.hh"
#include "log4cpp/Category.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/RollingFileAppender.hh"

#define setLogMsg(msg)                                                         \
    std::string("[")                                                           \
        .append(__FILE__)                                                      \
        .append(":")                                                           \
        .append(__func__)                                                      \
        .append(":")                                                           \
        .append(std::to_string(__LINE__))                                      \
        .append("]")                                                           \
        .append(msg)                                                           \
        .c_str()

#define Logemerg(msg) Mylogger::getLogger()->emerg(setLogMsg(msg))
#define Logfatal(msg) Mylogger::getLogger()->fatal(setLogMsg(msg))
#define Logalert(msg) Mylogger::getLogger()->alert(setLogMsg(msg))
#define Logerror(msg) Mylogger::getLogger()->error(setLogMsg(msg))
#define Logwarn(msg) Mylogger::getLogger()->warn(setLogMsg(msg))
#define Lognotice(msg) Mylogger::getLogger()->notice(setLogMsg(msg))
#define Loginfo(msg) Mylogger::getLogger()->info(setLogMsg(msg))
#define Logdebug(msg) Mylogger::getLogger()->debug(setLogMsg(msg))
class Mylogger {

    // class AutoRelease {
    //   public:
    //     ~AutoRelease() { loggerDestoryer(); }
    // };

  public:
    static Mylogger *getLogger();

    void emerg(const char *msg);
    void fatal(const char *msg);
    void alert(const char *msg);
    void error(const char *msg);
    void warn(const char *msg);
    void notice(const char *msg);
    void info(const char *msg);
    void debug(const char *msg);

  private:
    Mylogger();
    ~Mylogger();
    static void init_r();
    static void loggerDestoryer();
    static Mylogger *logger;
    // static AutoRelease ar;
    static pthread_once_t _once;
    log4cpp::Category &root;
    log4cpp::Category &consoleLog;
    log4cpp::Category &fatalLog;
    log4cpp::Category &warnLog;
    log4cpp::Category &debugLog;
};
