#include "mylog.h"
#include <pthread.h>

Mylogger *Mylogger::logger = nullptr;
// Mylogger::AutoRelease Mylogger::ar;
pthread_once_t Mylogger::_once = PTHREAD_ONCE_INIT;

Mylogger::Mylogger()
    : root(log4cpp::Category::getRoot()),
      consoleLog(root.getInstance("consoleLog")),
      fatalLog(root.getInstance("fatalLog")),
      warnLog(root.getInstance("warnLog")),
      debugLog(root.getInstance("debugLog")) {
    log4cpp::Appender *consoleApp =
        new log4cpp::OstreamAppender("console", &std::cout);
    log4cpp::Appender *fatalFileApp =
        new log4cpp::FileAppender("fatalFile", "fatalFile.log");
    log4cpp::Appender *warnFileApp =
        new log4cpp::FileAppender("warnFile", "warnFile.log");
    log4cpp::Appender *debugFileApp = new log4cpp::RollingFileAppender(
        "debugFile", "debugFile.log", 1024 * 1024 * 8, 9);

    log4cpp::PatternLayout *ptrn1 = new log4cpp::PatternLayout();
    log4cpp::PatternLayout *ptrn2 = new log4cpp::PatternLayout();
    log4cpp::PatternLayout *ptrn3 = new log4cpp::PatternLayout();
    log4cpp::PatternLayout *ptrn4 = new log4cpp::PatternLayout();
    ptrn1->setConversionPattern("%d%n[%p] %m%n");
    ptrn2->setConversionPattern("%d%n[%p] %m%n");
    ptrn3->setConversionPattern("%d%n[%p] %m%n");
    ptrn4->setConversionPattern("%d%n[%p] %m%n");
    consoleApp->setLayout(ptrn1);
    fatalFileApp->setLayout(ptrn2);
    warnFileApp->setLayout(ptrn3);
    debugFileApp->setLayout(ptrn4);

    consoleLog.setPriority(log4cpp::Priority::WARN);
    consoleLog.addAppender(consoleApp);

    fatalLog.setPriority(log4cpp::Priority::FATAL);
    fatalLog.addAppender(fatalFileApp);

    warnLog.setPriority(log4cpp::Priority::WARN);
    warnLog.addAppender(warnFileApp);

    debugLog.setPriority(log4cpp::Priority::DEBUG);
    debugLog.addAppender(debugFileApp);
}

Mylogger::~Mylogger() { log4cpp::Category::shutdown(); }

void Mylogger::loggerDestoryer() {
    if (logger != nullptr) {
        delete logger;
    }
}

Mylogger *Mylogger::getLogger() {
    pthread_once(&_once, init_r);
    return logger;
}

void Mylogger::init_r() {
    logger = new Mylogger();
    atexit(loggerDestoryer);
}

void Mylogger::emerg(const char *msg) {
    this->consoleLog.emerg(msg);
    this->fatalLog.emerg(msg);
    this->warnLog.emerg(msg);
    this->debugLog.emerg(msg);
}
void Mylogger::fatal(const char *msg) {
    this->consoleLog.fatal(msg);
    this->fatalLog.fatal(msg);
    this->warnLog.fatal(msg);
    this->debugLog.fatal(msg);
}
void Mylogger::alert(const char *msg) {
    this->consoleLog.alert(msg);
    this->fatalLog.alert(msg);
    this->warnLog.alert(msg);
    this->debugLog.alert(msg);
}
void Mylogger::error(const char *msg) {
    this->consoleLog.error(msg);
    this->fatalLog.error(msg);
    this->warnLog.error(msg);
    this->debugLog.error(msg);
}
void Mylogger::warn(const char *msg) {
    this->consoleLog.warn(msg);
    this->fatalLog.warn(msg);
    this->warnLog.warn(msg);
    this->debugLog.warn(msg);
}
void Mylogger::notice(const char *msg) {
    this->consoleLog.notice(msg);
    this->fatalLog.notice(msg);
    this->warnLog.notice(msg);
    this->debugLog.notice(msg);
}
void Mylogger::info(const char *msg) {
    this->consoleLog.info(msg);
    this->fatalLog.info(msg);
    this->warnLog.info(msg);
    this->debugLog.info(msg);
}
void Mylogger::debug(const char *msg) {
    this->consoleLog.debug(msg);
    this->fatalLog.debug(msg);
    this->warnLog.debug(msg);
    this->debugLog.debug(msg);
}
