#include "loggerfactory.hpp"
#include <functional>

//LoggerFactory
/*******************************************************************************/
LoggerFactory* LoggerFactory::m_factory = nullptr;

LoggerFactory* LoggerFactory::instance() {
    static std::once_flag oc;
    std::call_once(oc, [&]() {
        m_factory = new LoggerFactory();
    });
    return m_factory;
}

LoggerFactory::LoggerFactory() {}

LoggerFactory::~LoggerFactory() {
    delete m_logConfer;
}

std::list<std::string> LoggerFactory::getAllLoggerName() {
    std::list<std::string> list;
    for (auto logger : m_loggers) {
        list.push_back(logger.first);
    }
    return list;
}

void LoggerFactory::initFromFile(const std::string& filename) {
    m_logConfer = new LogConfigurator;
    std::vector<log_config_t> confs;
    confs = m_logConfer->setJsonConf(filename);

    for (auto conf : confs) {
        Logger::sptr pLogger = initialize(conf.LoggerName, conf.outPutLevel);
        pLogger->setFormatter(conf.rowFormatter);
        pLogger->setJsonFormatter(conf.jsonFormatter);
        for(std::string str : conf.appenders) {
            if(str == "StdoutAppender") {
                pLogger->addAppender(new StdoutAppender());
            } else if(str == "RollFileAppender") {
                pLogger->addAppender(new RollFileAppender());
            } else if(str == "SingleFileAppender") {
                pLogger->addAppender(new SingleFileAppender(conf.singleFileName));
            } else if(str == "ZMQAppender") {
                pLogger->addAppender(new ZMQAppender(conf.inetAddr, std::to_string(conf.port)));
            } else if(str == "HTTPAppender") {
                pLogger->addAppender(new HTTPAppender(conf.inetAddr, conf.port));
            }
        }
    }
}

Logger::sptr LoggerFactory::initialize(const std::string& name, const LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    Logger::sptr pLogger = m_loggers[name];

    if(!pLogger) {
        pLogger = std::make_shared<Logger>(name, level);
        m_loggers[name] = pLogger;
    }

    return pLogger;
}

//AsLoggerFactory
/*******************************************************************************/
AsLoggerFactory* AsLoggerFactory::m_factory = nullptr;

AsLoggerFactory* AsLoggerFactory::instance() {
    static std::once_flag oc;
    std::call_once(oc, [&]() {
        m_factory = new AsLoggerFactory();
    });
    return m_factory;
}

AsLoggerFactory::AsLoggerFactory() {
    m_logConfer = new LogConfigurator();
}
AsLoggerFactory::~AsLoggerFactory() {
    delete m_logConfer;
}

std::list<std::string> AsLoggerFactory::getAllLoggerName() {
    std::list<std::string> list;
    for (auto AsLogger : m_loggers) {
        list.push_back(AsLogger.first);
    }
    return list;
}

void AsLoggerFactory::initFromFile(const std::string& filename) {
    m_logConfer = new LogConfigurator;
    std::vector<log_config_t> confs;
    confs = m_logConfer->setJsonConf(filename);

    for (auto conf : confs) {
        AsLogger::sptr pAsLogger = initialize(conf.LoggerName, conf.outPutLevel, conf.bufferSize);
        for(std::string str : conf.appenders) {
            if(str == "StdoutAppender") {
                pAsLogger->addAppender(new StdoutAppender());
            } else if(str == "RollFileAppender") {
                pAsLogger->addAppender(new RollFileAppender());
            } else if(str == "SingleFileAppender") {
                pAsLogger->addAppender(new SingleFileAppender(conf.singleFileName));
            } else if(str == "ZMQAppender") {
                pAsLogger->addAppender(new ZMQAppender("tcp://" + conf.inetAddr
                                                       + ":" + std::to_string(conf.port)));
            } else if(str == "HTTPAppender") {
                pAsLogger->addAppender(new HTTPAppender(conf.inetAddr, conf.port));
            }
        }
    }
}

AsLogger::sptr AsLoggerFactory::initialize(const std::string & name, const LogLevel level, size_t size) {
    std::lock_guard<std::mutex> lock(m_mutex);
    AsLogger::sptr pAsLogger = m_loggers[name];

    if(!pAsLogger) {
        pAsLogger = std::make_shared<AsLogger>(name, level, size);
        m_loggers[name] = pAsLogger;
    }

    return pAsLogger;
}
