#include "logevent.hpp"

LogEvent::LogEvent() {}
LogEvent::LogEvent(const std::string& loggerName, LogLevel level,
                   const std::string& msg, const LocationInfo& locationInfo)
    : m_loggerName(loggerName),
      m_level(level),
      m_content(msg),
      m_locationInfo(locationInfo) {}
