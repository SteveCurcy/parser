/*
 * @author Xu.Cao
 * @date   2023-04-25
 * @detail 常用函数集合
 *
 * @history
 *      <author>    <date>      <version>       <description>
 *      Xu.Cao      2023-04-25  0.0.1           创建此文件
 */

#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <string>
#include <algorithm>

namespace utils {
    inline bool is_alpha(char c);

    inline bool is_upper(char c);

    inline bool is_lower(char c);

    inline bool is_digit(char c);

    inline bool is_alpha(const std::string &s);

    inline bool is_digit(const std::string &s);

    inline std::string to_upper(const std::string &s);

    inline std::string to_lower(const std::string &s);

    bool is_alpha(char c) {
        return ('a' <= c && 'z' >= c) || ('A' <= c && 'Z' >= c);
    }

    bool is_upper(char c) {
        return 'A' <= c && 'Z' >= c;
    }

    bool is_lower(char c) {
        return 'a' <= c && 'z' >= c;
    }

    bool is_digit(char c) {
        return '0' <= c && '9' >= c;
    }

    bool is_alpha(const std::string &s) {
        return std::all_of(s.begin(), s.end(), [](char c) { return is_alpha(c);});
    }

    bool is_digit(const std::string &s) {
        return std::all_of(s.begin(), s.end(), [](char c) { return is_digit(c);});
    }

    std::string to_upper(const std::string &s) {
        std::string ans;

        for (char c: s) {
            if (is_lower(c)) {
                ans.push_back(char(c - 32));
            } else {
                ans.push_back(c);
            }
        }

        return ans;
    }

    std::string to_lower(const std::string &s) {
        std::string ans;

        for (char c: s) {
            if (is_lower(c)) {
                ans.push_back(char(c + 32));
            } else {
                ans.push_back(c);
            }
        }

        return ans;
    }
}

#endif //PARSER_UTILS_H
