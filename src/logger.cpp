#include "logger.hpp"

namespace DAQ {

void Logger::log(LogLevel level, const std::string& msg) {
    if (level >= m_conf.outPutLevel) {
        auto self(shared_from_this());
        LogEvent::sptr event(new LogEvent(self->getName(), level, msg, LocationInfo::getLocationUnavailable()));
        for(auto it = m_appendersMap.begin() ; it != m_appendersMap.end(); ++it) {
            it->second->append(event);
        }
    }
}

void Logger::log(LogLevel level, const std::string& msg, const LocationInfo& location) {
    if (level >= m_conf.outPutLevel) {
        auto self(shared_from_this());
        LogEvent::sptr event(new LogEvent(self->getName(), level, msg, location));
        for(auto it = m_appendersMap.begin(); it != m_appendersMap.end(); ++it) {
            it->second->append(event);
        }
    }
}

void Logger::trace(const std::string& msg, const LocationInfo& location) {
    log(LogLevel::TRACE, msg, location);
}

void Logger::trace(const std::string& msg) {
    log(LogLevel::TRACE, msg);
}

void Logger::debug(const std::string& msg, const LocationInfo& location) {
    log(LogLevel::DEBUG, msg, location);
}

void Logger::debug(const std::string& msg) {
    log(LogLevel::DEBUG, msg);
}

void Logger::info(const std::string& msg, const LocationInfo& location) {
    log(LogLevel::INFO, msg, location);
}

void Logger::info(const std::string& msg) {
    log(LogLevel::INFO, msg);
}

void Logger::warn(const std::string& msg, const LocationInfo& location) {
    log(LogLevel::WARN, msg, location);
}

void Logger::warn(const std::string& msg) {
    log(LogLevel::WARN, msg);
}

void Logger::error(const std::string& msg, const LocationInfo& location) {
    log(LogLevel::ERROR, msg, location);
}

void Logger::error(const std::string& msg) {
    log(LogLevel::ERROR, msg);
}

void Logger::fatal(const std::string& msg, const LocationInfo& location) {
    log(LogLevel::FATAL, msg, location);
}

void Logger::fatal(const std::string& msg) {
    log(LogLevel::FATAL, msg);
}

void Logger::addAppender(Appender* appender) {
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    std::string id = appender->getId();
    auto it = m_appendersMap.find(id);
    if (it == m_appendersMap.end()) {
        if (!appender->hasFormatter()) {
            if (id.find("HTTPAppender") != std::string::npos) {
                appender->setFormatter(m_jsonFormatter);
            } else {
                appender->setFormatter(m_formatter);
            }
        }
        m_appendersMap[id] = appender;
    }
}

void Logger::delAppender(Appender* appender) {
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    std::string id = appender->getId();
    auto it = m_appendersMap.find(id);
    if (it != m_appendersMap.end()) {
        delete it->second;
        m_appendersMap.erase(it);
    }
}

void Logger::clearAppender() {
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    for (auto e : m_appendersMap) {
        delete e.second;
    }
    m_appendersMap.clear();
}

//AsLogger
/*******************************************************************************/
void AsLogger::pullEvent(moodycamel::ConcurrentQueue<LogEvent::sptr>& buffer,
                         std::map<std::string, Appender*>& appenders) {
    std::thread t([&]() {
        LogEvent::sptr event;
        while(true) {
            if(buffer.try_dequeue(event)) {
                for (auto it = appenders.begin(); it != appenders.end(); ++it) {
                    it->second->append(event);
                }
            }
        }
    });
    try {
        t.detach();
    } catch(const std::system_error& e) {
        std::cout << e.what() << "\n" << std::endl;
    }
}

void AsLogger::log(LogLevel level, const std::string & msg) {
    if (level >= m_conf.outPutLevel) {
        auto self(std::dynamic_pointer_cast<AsLogger>(shared_from_this()));
        LogEvent::sptr event(new LogEvent(self->getName(), level, msg, LocationInfo::getLocationUnavailable()));
        m_buffer.try_enqueue(event);
    }
}

void AsLogger::log(LogLevel level, const std::string & msg, const LocationInfo & location) {
    if (level >= m_conf.outPutLevel) {
        auto self(std::dynamic_pointer_cast<AsLogger>(shared_from_this()));
        LogEvent::sptr event(new LogEvent(self->getName(), level, msg, location));
        m_buffer.try_enqueue(event);
    }
}

}
