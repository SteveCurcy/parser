#ifndef PARSER_ARGS_H
#define PARSER_ARGS_H

#include "value.h"

namespace parser {
    inline char to_upper(char _c) {
        return 'a' <= _c && _c <= 'z' ? char(_c - 32) : _c;
    }

    inline char to_lower(char _c) {
        return 'A' <= _c && _c <= 'Z' ? char(_c + 32) : _c;
    }

    void to_upper(std::string& _s) {
        for (int i = 0; i < _s.size(); i++) {
            _s[i] = to_upper(_s[i]);
        }
    }

    void to_lower(std::string& _s) {
        for (int i = 0; i < _s.size(); i++) {
            _s[i] = to_lower(_s[i]);
        }
    }

    class args {
    public:
        typedef value::name_type    name_type;
        typedef const char*         desc_type;
        typedef value::value_type   data_type;
        typedef parser::value       value_type;
        typedef value::type_type    type_type;

    protected:
        name_type  m_name;  // 参数的全名
        desc_type  m_desc;  // 对该参数的描述
        value_type m_value;

    public:
        args() = default;

        explicit args(name_type _name): m_name(_name) {
            if (_name[0] == '-') {
                m_value.set_name(_name + 2);
            } else {
                m_value.set_name(_name);
            }
        }

        args(name_type _name, desc_type _desc): m_name(_name), m_desc(_desc) {
            if (_name[0] == '-') {
                m_value.set_name(_name + 2);
            } else {
                m_value.set_name(_name);
            }
        }

        args(name_type _name, desc_type _desc, type_type _type): m_name(_name), m_desc(_desc) {
            if (_name[0] == '-') {
                m_value.set_name(_name + 2);
            } else {
                m_value.set_name(_name);
            }
            m_value.set_type(_type);
        }

        void set_value_type(type_type _type) { m_value.set_type(_type); }

        void set_value_name(name_type _name) { m_value.set_name(_name); }

        void set(data_type _value) { m_value.set(_value); }

        value_type get() { return m_value; }

        void show_usage();

        void show_help();
    };

    inline void args::show_usage() {
        std::string _value_name = m_value.get_name();
        to_lower(_value_name);
        printf("%s ", _value_name.c_str());
        to_upper(_value_name);
        printf("%s", _value_name.c_str());
    }

    inline void args::show_help() {
        show_usage();
        printf("\t\t\t%s", m_desc);
    }

    class op_args: public args {
    public:
        typedef value::name_type    name_type;
        typedef const char*         desc_type;
        typedef value::value_type   data_type;
        typedef parser::value       value_type;
        typedef value::type_type    type_type;

    protected:
        name_type m_short_name;
    };
}

#endif //PARSER_ARGS_H
