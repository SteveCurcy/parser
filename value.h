#ifndef PARSER_VALUE_H
#define PARSER_VALUE_H

#include <ostream>
#include <string>
#include <cstring>

namespace parser {
    constexpr unsigned int TYPE_STRING = 0;
    constexpr unsigned int TYPE_INT = 1;
    constexpr unsigned int TYPE_FLOAT = 2;
    constexpr unsigned int TYPE_BOOL = 3;

    template<typename... Args>
    inline void p_assert(bool _exp, const char *_fmt, Args&&... _args) {
        if (!_exp) {
            printf(_fmt, std::forward<Args>(_args)...);
            abort();
        }
    }

    /**
     * value 类用于保存一个任意类型的值，目前支持的类型包括：
     * - string 任意类型都可以转换为 string 类型（BOOL 类型可以将其实际存储的内容转为字符串输出）；
     * - int 整数类型，可以与 float 相互转换；
     * - float 浮点数类型，可以与 int 相互转换；
     * - bool 类型，记录当前类型是否出现过，或者被设置，表示一种二元状态，即被赋值过即为 true，否则为 false
     *
     * value 的真实数据内容保存在一个 char[] 中，保证不为 nullptr，默认为 ""
     */
    class value {
    public:
        typedef unsigned int type_type;
        typedef const char*  name_type;
        typedef const char*  data_type;
        typedef size_t       size_type;

    protected:
        /**
         * 所有成员变量都应该是只读的，保证它的不变性，这是十分重要的。
         * 想要读取本类的值只能通过赋值操作，从而得到一个值的备份，而不能通过其他方式，以此来保证其不可变性。
         */
        name_type m_name;
        type_type m_type = TYPE_STRING;
        data_type m_data = "";    // 默认值

        /**
         * 获取传入字符串可以解析得到的数据类型，
         * BOOL 类型不需要判断，因为只要赋值就为 true，否则为 false
         * @param _data 要获取类型的字符串
         * @return 解析得到的类型
         */
        static type_type get_type(data_type _data) {
            enum { NIL, NUM, STRING, FLOAT } _type = NIL;
            size_type _value_size = strlen(_data);
            for (int i = 0; i < _value_size; i++) {
                if ('0' <= _data[i] && _data[i] <= '9') {
                    switch (_type) {
                        case NIL:
                            _type = NUM;
                            break;
                    }
                } else if (_data[i] == '.') {
                    switch (_type) {
                        case NIL:
                        case FLOAT:
                            _type = STRING;
                            break;
                        case NUM:
                            _type = FLOAT;
                            break;
                    }
                } else {
                    _type = STRING;
                }
                if (_type == STRING) {
                    break;
                }
            }
            return _type;
        }

        /**
         * 查看传入值是否与预期类型兼容或匹配
         * @param _data 用户传入参数的值
         * @return 是否兼容/匹配
         */
        bool check_type(data_type _data) {
            enum { NIL, NUM, STRING, FLOAT };
            int _type = get_type(_data);
            switch (_type) {
                case NUM:
                    return true;
                case FLOAT:
                    if (m_type == TYPE_INT) {
                        printf("\033[33mConvert Float to Int may narrow the type!\033[0m\n");
                    }
                    return true;
                case STRING:
                    p_assert(m_type == TYPE_STRING, "\033[31m\"%s\" want a number but get a string!\033[0m\n", m_name);
            }
        }

        /**
         * 查看当前参数能否转换为目标类型，如果不能将报错
         * @param _type 目标类型
         */
        void check_convert(type_type _type) {
            switch (m_type) {
                case TYPE_STRING:
                    p_assert(_type == TYPE_STRING || _type == TYPE_BOOL, "\033[31m\"%s\" cannot be converted to number!\033[0m\n", m_name);
                    break;
                case TYPE_BOOL:
                    p_assert(_type == TYPE_BOOL || _type == TYPE_STRING, "\033[31m\"%s\" can only be converted to bool or string!\033[0m\n", m_name);
                    break;
                case TYPE_FLOAT:
                case TYPE_INT:
                    break;
            }
        }

    public:
        value() {}
        value(const char *_name): m_name(_name) {}
        value(const char *_name, const char *_value): m_name(_name), m_data(_value) {}
        value(const char *_name, const char *_value, type_type _type): m_name(_name), m_data(_value), m_type(_type) {}

        void set_name(name_type _name) { m_name = _name; }

        bool set() { return m_type == TYPE_BOOL && (m_data = "true"); }

        /** 设置 value 的数据内容，在设置前确保设置的内容不为空 */
        bool set(data_type _data) {
            if (_data && _data[0] != '\0' && (m_type == TYPE_BOOL || check_type(_data))) {
                m_data = _data;
                return true;
            }
            return false;
        }

        void set_type(const type_type _type) { m_type = _type; }

        type_type get_type() const { return m_type; }

        std::string get_name() const {
            return m_name;
        }

        operator bool() {
            check_convert(TYPE_BOOL);
            return m_data[0] != '\0';
        }

        operator int() {
            check_convert(TYPE_INT);
            char *tmp;
            return (int)strtol(m_data, &tmp, 10);
        }

        operator float() {
            check_convert(TYPE_FLOAT);
            char *tmp;
            return strtof(m_data, &tmp);
        }

        operator std::string() {
            check_convert(TYPE_STRING);
            return m_data;
        }

        friend std::ostream &operator<<(std::ostream &out, const value& _val) {
            out << _val.m_data;
            return out;
        }
    };
}

#endif //PARSER_VALUE_H
