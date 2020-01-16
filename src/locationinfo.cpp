#include "locationinfo.hpp"

namespace daq {

const char* const LocationInfo::NA = "??";
const char* const LocationInfo::NA_METHOD = "??::??";

const LocationInfo& LocationInfo::getLocationUnavailable() {
    static const LocationInfo unavailable;
    return unavailable;
}

LocationInfo::LocationInfo(const char * const fileName,
                           const char * const methodName,
                           int lineNumber)
    : m_lineNumber(lineNumber),
      m_fileName(fileName),
      m_methodName(methodName) {}

LocationInfo::LocationInfo()
    : m_lineNumber(-1),
      m_fileName(LocationInfo::NA),
      m_methodName(LocationInfo::NA_METHOD) {
}

LocationInfo::LocationInfo(const LocationInfo & rth)
    :  m_lineNumber(rth.m_lineNumber),
       m_fileName(rth.m_fileName),
       m_methodName(rth.m_methodName) {
}

LocationInfo& LocationInfo::operator=(const LocationInfo& rth) {
    m_fileName = rth.m_fileName;
    m_methodName = rth.m_methodName;
    m_lineNumber = rth.m_lineNumber;
    return *this;
}

LocationInfo& LocationInfo::operator=(const LocationInfo&& rth) {
    m_fileName = rth.m_fileName;
    m_methodName = rth.m_methodName;
    m_lineNumber = rth.m_lineNumber;
    return *this;
}

void LocationInfo::clear() {
    m_fileName = NA;
    m_methodName = NA_METHOD;
    m_lineNumber = -1;
}


const char* LocationInfo::getFileName() const {
    return m_fileName;
}

int LocationInfo::getLineNumber() const {
    return m_lineNumber;
}

const std::string LocationInfo::getMethodName() const {
    std::string tmp(m_methodName);
    size_t colonPos = tmp.find("::");
    if (colonPos != std::string::npos) {
        tmp.erase(0, colonPos + 2);
    } else {
        size_t spacePos = tmp.find(' ');
        if (spacePos != std::string::npos) {
            tmp.erase(0, spacePos + 1);
        }
    }
    //size_t parenPos = tmp.find('(');
    //if (parenPos != std::string::npos) {
    //    tmp.erase(parenPos);
    //}
    return tmp;
}

const std::string LocationInfo::getClassName() const {
    std::string tmp(m_methodName);
    size_t colonPos = tmp.find("::");
    if (colonPos != std::string::npos) {
        tmp.erase(colonPos);
        size_t spacePos = tmp.find_last_of(' ');
        if (spacePos != std::string::npos) {
            tmp.erase(0, spacePos + 1);
        }
        return tmp;
    }
    //tmp.erase(0, tmp.length());
    return "??";
}

}
