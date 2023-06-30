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
        typedef args::desc_type desc_type;
        typedef args::type_type type_type;

    private:
        int m_argc;
        char **m_argv;
        std::unordered_map<std::string, args> pos;
        std::unordered_map<std::string, option> opt;

        void show_usage() const {
            printf("Usage: %s ", m_argv[0]);
            for (const auto& arg: pos) {
                arg.second.show_usage();
                printf(" ");
            }
            for (const auto& arg: opt) {
                arg.second.show_usage();
                printf(" ");
            }
            printf("[-h|--help]\n");
        }

        void show_help() const {
            show_usage();
            printf("\nPOSITIONAL ARGUMENTS: \n");
            for (const auto& arg: pos) {
                arg.second.show_help();
            }
            printf("\nOPTIONAL ARGUMENTS: \n");
            for (const auto& arg: opt) {
                arg.second.show_help();
            }
            printf("[-h|--help]\t\t\tShow this help message\n");
        }

    public:
        parser(int _argc, char *_argv[]) { m_argc = _argc; m_argv = _argv; }

        static parser& get_instance(int _argc, char *_argv[]) { static parser instance(_argc, _argv); return instance; }

        void parse() {
            for (int i = 1; i < m_argc; i++) {
                if (m_argv[i][0] == '-') {
                    if (!strcmp(m_argv[i], "-h") || !strcmp(m_argv[i], "--help")) {
                        show_help();
                        exit(0);
                    }
                    // optional
                    for (auto& p: opt) {
                        if (!strcmp(p.second.get_short_name(), m_argv[i]) || !strcmp(p.second.get_name(), m_argv[i])) {
                            if (p.second.get_type() == TYPE_BOOL) {
                                p.second.set();
                            } else {
                                if (i + 1 < m_argc)
                                    p.second.set(m_argv[++i]);
                                else {
                                    p_assert(false, "\033[31m%s value is not given!\033[0m\n", m_argv[i]);
                                }
                            }
                            break;
                        }
                    }
                } else {
                    // positional
                    for (auto& p: pos) {
                        if (!strcmp(p.second.get_name(), m_argv[i])) {
                            if (i + 1 < m_argc)
                                p.second.set(m_argv[++i]);
                            else {
                                p_assert(false, "\033[31m%s value is not given!\033[0m\n", m_argv[i]);
                            }
                            break;
                        }
                    }
                }
            }
            for (const auto& p: pos) {
                p_assert(p.second.get_flag(), "\033[31m%s is not been set yet!\033[0m\n", p.second.get_name());
            }
        }

        void add_argument(name_type _name) {
            if (_name[0] == '-') {
                opt.emplace(_name + 2, option(_name));
            } else {
                pos.emplace(_name, args(_name));
            }
        }

        /**
         * 添加一个参数，分别指定短名和参数全名；
         * 当短名为空时则创建位置参数；
         * 短名不为空则创建可选参数；
         * 用户可以通过短名传参来控制创建的参数类型
         *
         * @param _short_name 短名
         * @param _name 全名
         */
        void add_argument(name_type _short_name, name_type _name) {
            if (_short_name) {
                opt.emplace(_name + 2, option(_short_name, _name));
            } else {
                pos.emplace(_name, args(_name));
            }
        }

        void add_argument(name_type _short_name, name_type _name, desc_type _desc) {
            if (_short_name) {
                opt.emplace(_name + 2, option(_short_name, _name, _desc));
            } else {
                pos.emplace(_name, args(_name, _desc));
            }
        }

        void add_argument(name_type _short_name, name_type _name, desc_type _desc, type_type _type) {
            if (_short_name) {
                opt.emplace(_name + 2, option(_short_name, _name, _desc, _type));
            } else {
                pos.emplace(_name, args(_name, _desc, _type));
            }
        }

        void add_argument(name_type _short_name, name_type _name, desc_type _desc, type_type _type, name_type _var_name) {
            if (_short_name) {
                opt.emplace(_var_name, option(_short_name, _name, _desc, _type, _var_name));
            } else {
                pos.emplace(_var_name, args(_name, _desc, _type, _var_name));
            }
        }

        value_type operator[](ckey_reference _key) {
            auto pos_it = pos.find(_key);
            if (pos_it != pos.end()) return pos_it->second.get();
            auto opt_it = opt.find(_key);
            if (opt_it != opt.end()) return opt_it->second.get();
            printf("\033[31m%s is a invalid argument!\033[0m\n", _key.c_str());
            abort();
        }
    };
}

#endif //PARSER_PARSER_H
