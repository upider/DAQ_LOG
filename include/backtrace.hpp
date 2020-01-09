#ifndef __BACKTRACE_HPP_
#define __BACKTRACE_HPP_

#include <vector>
#include <string>
#include <sstream>
#include <execinfo.h>

namespace DAQ {

/// \brief Backtrace 获取函数栈信息
///
/// \param bt 保存栈的数组
/// \param size 栈层数
/// \param skip 跳过栈层数
void Backtrace(std::vector<std::string>& bt, int size, int skip) {
    void** array = (void**)malloc(sizeof(void*)*size);
    size_t s = ::backtrace(array, size);
    char** strings = backtrace_symbols(array, s);

    if (strings == nullptr) {
        return;
    }

    for (size_t i = skip; i < s; ++i) {
        bt.push_back(strings[i]);
    }

    free(strings);
    free(array);
}

/// \brief BacktraceToString 把栈信息转化为string
///
/// \param bt 保存栈的数组
/// \param size 栈层数
/// \param skip 跳过栈层数
/// \param prefix 前缀
///
/// \return 栈信息string
std::string BacktraceToString(int size, int skip, const std::string& prefix) {
    std::vector<std::string> bt;
    Backtrace(bt, size, skip);
    std::stringstream ss;
    for (size_t i = 0; i < bt.size(); ++i) {
        ss << prefix << bt[i] << std::endl;
    }
    return ss.str();
}

}
#endif /*__BACKTRACE_HPP_*/
