#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include <string>
#include <unordered_map>
#include "args.h"

namespace parser {
    class parser {
    public:
        typedef const char*     name_type;
        typedef std::string     key_type;
        typedef const key_type& ckey_reference;
        typedef value           value_type;

    private:
        int m_argc;
        char **m_argv;
        std::unordered_map<key_type, args> m_values;

    public:
        parser() = default;

        static parser& get_instance() { static parser instance; return instance; }

        void parse(int _argc, char *_argv[]) { m_argc = _argc; m_argv = _argv; }

        value_type operator[](ckey_reference _key) {
            auto _value_it = m_values.find(_key);
            p_assert(_value_it == m_values.end(), "\033[31m%s is not existent!\033[0m\n", _key.c_str());
            return _value_it->second;
        }
        
        void add_argument(name_type _arg_name) {
            
        }
    };
}

#endif //PARSER_PARSER_H
