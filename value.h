#ifndef PARSER_VALUE_H
#define PARSER_VALUE_H

#include <ostream>
#include <string>

namespace parser {
    constexpr unsigned int TYPE_STRING = 0;
    constexpr unsigned int TYPE_INT = 1;
    constexpr unsigned int TYPE_FLOAT = 2;

    template<typename... Args>
    inline void p_assert(bool _exp, const char *_fmt, Args&&... _args) {
        if (!_exp) {
            printf(_fmt, std::forward<Args>(_args)...);
            abort();
        }
    }

    class value {
    public:
        typedef unsigned int type_type;
        typedef const char*  name_type;
        typedef const char*  value_type;

    protected:
        name_type m_name;
        type_type m_type = TYPE_STRING;
        value_type m_value = "";    // 默认值

    public:
        value() {}
        value(const char *_name): m_name(_name) {}
        value(const char *_name, const char *_value): m_name(_name), m_value(_value) {}
        value(const char *_name, const char *_value, type_type _type): m_name(_name), m_value(_value), m_type(_type) {}

        void set_name(const char *_name) { m_name = _name; }
        void set(value_type _value) {
            if (_value) {
                m_value = _value;
            }
        }
        void set_type(const type_type _type) { m_type = _type; }

        std::string get_name() {
            return {m_name};
        }

        operator int() {
            p_assert(m_type == TYPE_INT, "\033[31m%s is not int value!\033[0m", m_name);
            char *tmp;
            return (int)strtol(m_value, &tmp, 10);
        }

        operator float() {
            p_assert(m_type == TYPE_FLOAT, "\033[31m%s is not float value!\033[0m", m_name);
            char *tmp;
            return strtof(m_value, &tmp);
        }

        operator std::string() {
            p_assert(m_type == TYPE_STRING, "\033[31m%s is not string value!\033[0m", m_name);
            return {m_value};
        }

        friend std::ostream &operator<<(std::ostream &out, const value& _val) {
            out << _val.m_value;
            return out;
        }
    };
}

#endif //PARSER_VALUE_H
