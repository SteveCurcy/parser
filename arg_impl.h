/*
 * @author Xu.Cao
 * @date   2023-04-24
 * @detail 可选参数，继承参数基类，默认数据类型是 bool，选中则为 true
 *
 * @history
 *      <author>    <date>      <version>       <description>
 *      Xu.Cao      2023-04-24  0.0.1           创建此文件
 */

#ifndef ARGS_PARSER_OPTIONAL_ARG_H
#define ARGS_PARSER_OPTIONAL_ARG_H

#include <iostream>
#include "argument.h"

template<typename T = bool>
class arg_impl : public argument {
    T m_value;
    std::string m_small_name;
public:

    arg_impl(const std::string &small_name_, const std::string &full_name_,
             const std::string &description_);

    arg_impl() = default;

    ~arg_impl() override = default;

    void show_usage() override;

    void show_help() override;

    bool is_this(const std::string &arg_name_) override;

    T &get();

    void set(const T &t);
};

template<typename T>
arg_impl<T>::arg_impl(const std::string &small_name_, const std::string &full_name_,
                      const std::string &description_) :
        m_small_name(small_name_), argument(full_name_, description_) {

    if (typeid(T) == typeid(bool)) {
        m_var_type = ARG_BOOL;
    } else if (typeid(T) == typeid(int)) {
        m_var_type = ARG_INT;
    } else if (typeid(T) == typeid(std::string)) {
        m_var_type = ARG_STRING;
    } else {
        throw std::runtime_error("当前只支持 bool, int, std::string。");
    }

    m_var_name = full_name_[0] == '-' ? full_name_.substr(2) : full_name_;
}

template<typename T>
void arg_impl<T>::show_usage() {
    std::cout << " [";
    if (!m_small_name.empty()) {
        std::cout << m_small_name;
    } else {
        std::cout << m_full_name;
    }
    if (m_var_type != ARG_BOOL && !m_var_name.empty()) {
        std::cout << " " << utils::to_upper(m_var_name) << "]";
    } else {
        std::cout << "]";
    }
    std::cout << std::flush;
}

template<typename T>
void arg_impl<T>::show_help() {
    std::string help_msg_ = m_small_name;
    if (!m_small_name.empty()) {
        help_msg_ += ", " + m_full_name + (m_var_type != ARG_BOOL ? " " + utils::to_upper(m_var_name) : "");
    } else {
        help_msg_ += m_full_name + (m_var_type != ARG_BOOL ? " " + utils::to_upper(m_var_name) : "");
    }
    std::cout << help_msg_;

    std::string left_blanks = "\n\t\t\t\t";
    int rest_blanks_size_ = 32 - int(help_msg_.size());
    if (!m_description.empty()) {
        if (rest_blanks_size_ < 4) {
            std::cout << left_blanks;
        } else {
            std::cout << std::string(rest_blanks_size_, ' ');
        }

        for (int i = 0; i < m_description.size(); i += 32) {
            if (i) {
                std::cout << left_blanks << m_description.substr(i, 32);
            } else {
                std::cout << m_description.substr(0, 32);
            }
        }
    }
    std::cout << std::endl;
}

template<typename T>
bool arg_impl<T>::is_this(const std::string &arg_name_) {
    return (arg_name_ == m_small_name) || (arg_name_ == m_full_name);
}

template<typename T>
T &arg_impl<T>::get() {
    return m_value;
}

template<typename T>
void arg_impl<T>::set(const T &t) {
    m_value = t;
    f_is_set = true;
}

#endif //ARGS_PARSER_OPTIONAL_ARG_H
