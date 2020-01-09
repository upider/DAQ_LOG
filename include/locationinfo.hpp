#ifndef __LOCATIONINFO_HPP_
#define __LOCATIONINFO_HPP_

#include <string>

namespace DAQ {

class LocationInfo {
    public:
        /**
         * 得不到文件名或行号就返回空
          */
        static const char* const NA;
        static const char* const NA_METHOD;

        /// @brief getLocationUnavailable 不使用LocationInfo时的位置信息
        ///
        /// @return LocationInfo
        static const LocationInfo& getLocationUnavailable();



        LocationInfo( const char* const fileName,
                      const char* const functionName,
                      int lineNumber);

        LocationInfo();
        LocationInfo(const LocationInfo& src);
        LocationInfo & operator = (const LocationInfo& src);

        void clear();

        const std::string getClassName() const;
        const char* getFileName() const;
        int getLineNumber() const;
        const std::string getMethodName() const;


    private:
        int m_lineNumber;
        const char* m_fileName;
        const char* m_methodName;


};

}

#define LOCATIONINFO LocationInfo(__FILE__, __PRETTY_FUNCTION__, __LINE__)

#endif /*__LOCATIONINFO_HPP_*/
