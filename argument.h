/*
 * @author Xu.Cao
 * @date   2023-04-24
 * @detail main 函数参数的抽象，对一个参数的描述
 *
 * @history
 *      <author>    <date>      <version>       <description>
 *      Xu.Cao      2023-04-24  0.0.1           创建此文件
 */

#ifndef ARGS_PARSER_ARG_BASE_H
#define ARGS_PARSER_ARG_BASE_H

#include <string>
#include "utils.h"

extern constexpr int ARG_BOOL = 0;
extern constexpr int ARG_INT = 1;
extern constexpr int ARG_STRING = 2;

class argument {
protected:
    int m_var_type = ARG_BOOL;
    bool f_is_set = false;
    std::string m_var_name; // 参数对应变量的名称
    std::string m_full_name; // 参数的完整名字，如 --help，但只记录 help
    std::string m_description;  // 对参数的描述
public:
    static std::string types[];

    virtual void show_usage() = 0;

    virtual void show_help() = 0;

    virtual bool is_this(const std::string &arg_name_) = 0;

    int get_type() const {
        return m_var_type;
    }

    const std::string &get_var_name() const;

    bool has_set() const {
        return f_is_set;
    }

    argument(const std::string &arg_name_, const std::string &description_) :
            m_full_name(arg_name_), m_description(description_) {}

    virtual ~argument() = default;
};

std::string argument::types[] = {"bool", "int", "std::string"};

const std::string &argument::get_var_name() const {
    return m_var_name;
}

#endif //ARGS_PARSER_ARG_BASE_H
