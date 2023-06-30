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
        for (char & _ : _s) {
            _ = to_upper(_);
        }
    }

    void to_lower(std::string& _s) {
        for (char & _ : _s) {
            _ = to_lower(_);
        }
    }

    /**
     * 参数类型，一个参数包含参数名，对该参数的描述，以及保存的数据 value；
     * 为了方便获取位置参数的状态，设置了“当前变量是否被赋值”的标志；
     * 此外，用户还可以指定当前参数对应“变量”的名称，默认为参数名；
     * NOTE：不管一个参数是位置参数还是可选参数都必须有参数名，可选参数支持短名
     */
    class args {
    public:
        typedef value::name_type    name_type;
        typedef const char*         desc_type;
        typedef value::data_type    data_type;
        typedef parser::value       value_type;
        typedef value::type_type    type_type;
        typedef bool                flag_type;

    protected:
        name_type  m_name = "";  // 参数的全名
        desc_type  m_desc = "";  // 对该参数的描述
        value_type m_value;
        flag_type  f_is_set;    // 当前变量是否已经被赋值

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

        args(name_type _name, desc_type _desc, type_type _type, name_type _var_name): m_name(_name), m_desc(_desc) {
            m_value.set_name(_var_name);
            m_value.set_type(_type);
        }

        void set_type(type_type _type) { m_value.set_type(_type); }

        void set_value_name(name_type _name) { m_value.set_name(_name); }

        void add_description(desc_type _desc) { m_desc = _desc; }

        void set() { f_is_set = m_value.set(); }

        name_type get_name() const { return m_name; }

        type_type get_type() const { return m_value.get_type(); }

        flag_type get_flag() const { return f_is_set; }

        void set(data_type _value) {
            p_assert(_value, "\033[31m数据赋值空指针!\033[0m");
            f_is_set = m_value.set(_value);
        }

        /** 对于位置参数，get 时一定要确保用户传入过；而对于可选参数，可以有默认值，则不必校验 */
        virtual value_type get() {
//            p_assert(f_is_set, "\033[31m\"%s\" is not set!\033[0m\n", m_value.get_name().c_str());
            return m_value;
        }

        virtual void show_usage() const;

        void show_help() const;
    };

    inline void args::show_usage() const {
        printf("%s", m_name);
        if (m_value.get_type() != TYPE_BOOL) {
            std::string _value_name = m_value.get_name();
            to_upper(_value_name);
            printf(" %s", _value_name.c_str());
        }
    }

    inline void args::show_help() const {
        show_usage();
        printf("\t\t\t%s\n", m_desc);
    }

    class option: public args {
    public:
        typedef value::name_type    name_type;
        typedef const char*         desc_type;
        typedef value::data_type    data_type;
        typedef parser::value       value_type;
        typedef value::type_type    type_type;

    protected:
        name_type m_short_name = "";

    public:
        option() = default;

        explicit option(name_type _name): args(_name) {}

        option(name_type _short_name, name_type _name): m_short_name(_short_name), args(_name) {}

        option(name_type _short_name, name_type _name, desc_type _desc): m_short_name(_short_name), args(_name, _desc) {}

        option(name_type _short_name, name_type _name, desc_type _desc, type_type _type): m_short_name(_short_name), args(_name, _desc, _type) {}

        option(name_type _short_name, name_type _name, desc_type _desc, type_type _type, name_type _var_name): m_short_name(_short_name), args(_name, _desc, _type, _var_name) {}

        void show_usage() const override {
            if (m_short_name[0] == '\0') {
                printf("[");
            } else {
                printf("[%s|", m_short_name);
            }
            args::show_usage();
            printf("]");
        }

//        value_type get() override { return m_value; }

        name_type get_short_name() const { return m_short_name; }
    };
}

#endif //PARSER_ARGS_H
