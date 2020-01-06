#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include <functional>
#include "formatter.hpp"

//Formatter
/*******************************************************************************/
Formatter::Formatter(const std::string& pattern): m_pattern(pattern) {
    patternParser();
}

std::string Formatter::format(LogEvent::sptr event) {
    std::stringstream ss;
    for (auto& f : m_items) {
        f->addFormat(ss, event);
    }
    return ss.str();
}

void Formatter::patternParser() {
    //tuple<str,fmt,type>
    //type = 0 -> 直接输出字符串
    //type = 1 -> 输出固定的格式%p...
    //type = 2 -> 输出带有格式的时间
    std::vector<std::tuple<std::string, std::string, int>> vec;
    for (unsigned int i = 0; i < m_pattern.size(); ++i) {
        std::string tmp;
        tmp += m_pattern[i];
        if (tmp != "%") {
            vec.emplace_back(std::move(tmp), "", 0);
        } else {
            tmp = m_pattern[++i];
            if (tmp == "%") {
                vec.emplace_back(std::move(tmp), "", 0);
            } else if(tmp == "d") {
                auto rihgt_brac_pos = m_pattern.find_first_of('}', ++i);
                auto time_str = m_pattern.substr(i + 1, rihgt_brac_pos - i - 1);
                vec.emplace_back("d", std::move(time_str), 2);
                i = rihgt_brac_pos;
            } else {
                vec.emplace_back(std::move(tmp), "", 1);
            }
        }
    }

    static std::map<std::string, std::function<FormatItem::sptr(const std::string str)>> s_fmt_items = {
#define XX(str, F) \
		{#str, [](const std::string fmt){ return FormatItem::sptr(new F(fmt));}} \

        XX(m, MessageFormatItem),
        XX(p, LevelFormatItem),
        XX(n, NewLineFormatItem),
        XX(t, ThreadIdFormatItem),
        XX(d, DateTimeFormatItem),
        XX(f, FileNameFormatItem),
        XX(l, LineNumberFormatItem),
        XX(T, TabFormatItem),
        XX(C, ClassFormatItem),
        XX(M, MethodFormatItem),
        XX(F, FiberFormatItem),
        XX(N, LoggerNameFormatItem),

#undef XX
    };

    for (auto& ele : vec) {
        if (std::get<2>(ele) == 0) {
            m_items.emplace_back(new StrFormatItem(std::get<0>(ele)));
        } else if(std::get<2>(ele) == 1) {
            auto it = s_fmt_items.find(std::get<0>(ele));
            if (it != s_fmt_items.end()) {
                m_items.emplace_back(it->second(std::get<1>(ele)));
            } else {
                m_items.emplace_back(new StrFormatItem("<Fmt Error> : %" + std::get<0>(ele)));
            }
        } else if(std::get<2>(ele) == 2) {
            m_items.emplace_back(new DateTimeFormatItem(std::get<1>(ele)));
        }
    }
}
