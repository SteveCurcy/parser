/*
 * @author Xu.Cao
 * @date   2023-04-24
 * @detail 基于 C++11 的 main 函数参数解析工具
 *
 * @history
 *      <author>    <date>      <version>       <description>
 *      Xu.Cao      2023-04-24  0.0.1           创建此文件
 */

#ifndef ARGS_PARSER_PARSER_H
#define ARGS_PARSER_PARSER_H

#include <iostream>
#include <vector>
#include "arg_impl.h"

class parser {
    class remover {
        ~remover() {
            delete parser::instance;
        }
    };

    parser() : m_helper("-h", "--help", "show this message") {}

    ~parser();

    parser(const parser &) = default;

    parser(parser &&) = default;

    parser &operator=(const parser &) = default;

    parser &operator=(parser &&) = default;

    argument *find_option_by_name(const std::string &name_);

    argument *find_position_by_name(const std::string &name_);

    static parser *instance;
    static remover remover;

    std::string m_prog;
    arg_impl<bool> m_helper;
    std::vector<argument *> m_options;
    std::vector<argument *> m_positions;

public:

    static parser &get_instance();

    void parse(int argc_, char *argv_[], const std::string &prog_ = "");

    void show_help();

    template<typename Val>
    void add_argument(const std::string &small_name_, const std::string &full_name_,
                      const std::string &desc_);

    template<typename Val>
    void add_argument(const std::string &arg_name_, const std::string &desc_);

    template<typename Val>
    Val &get(const std::string &);
};

parser *parser::instance = nullptr;

parser::~parser() {
    for (auto *opt: m_options) {
        delete opt;
    }
}

argument *parser::find_option_by_name(const std::string &name_) {
    for (auto *arg: m_options) {
        if (arg->is_this(name_)) {
            return arg;
        }
    }
    return nullptr;
}

argument *parser::find_position_by_name(const std::string &name_) {
    for (auto *arg: m_positions) {
        if (arg->is_this(name_)) {
            return arg;
        }
    }
    return nullptr;
}

parser &parser::get_instance() {
    if (nullptr == instance) {
        instance = new parser;
    }
    return *instance;
}

void parser::parse(int argc_, char **argv_, const std::string &prog_) {
    if (!prog_.empty()) {
        m_prog = prog_;
    } else {
        m_prog = std::string(argv_[0]);
    }
    if (argc_ <= 1) {
        return;
    }

    argument *arg_ = nullptr;
    for (int i = 1; i < argc_; i++) {
        if (argv_[i][0] == '-') {
            if (m_helper.is_this(argv_[i])) {
                show_help();
                exit(0);
            }
            arg_ = find_option_by_name(argv_[i]);
            if (nullptr == arg_) {
                std::cout << argv_[i][1] << std::endl;
                throw std::runtime_error("未知的参数：" + std::string(argv_[i]));
            }

            // 短名字，如果不是 bool 类型，则看参数的值
            if (arg_->get_type() == ARG_BOOL) {
                auto *opt = (arg_impl<bool> *) arg_;
                opt->set(true);
                continue;
            }
        } else {
            arg_ = find_position_by_name(argv_[i]);
            if (nullptr == arg_) {
                std::cout << argv_[i][1] << std::endl;
                throw std::runtime_error("未知的参数：" + std::string(argv_[i]));
            }
        }
        i++;
        if (i >= argc_) {
            throw std::runtime_error(
                    std::string(argv_[i - 1]) + " 需要一个 " + argument::types[arg_->get_type()] + " 参数输入！");
        }
        switch (arg_->get_type()) {
            case ARG_BOOL:
                if (std::string(argv_[i]) != "true" && std::string(argv_[i]) != "false") {
                    throw std::runtime_error(((arg_impl<int> *) arg_)->get_var_name() + " 的值只能为 true 或 false！");
                }
                ((arg_impl<bool> *) arg_)->set(argv_[i][0] == 't');
                break;
            case ARG_INT:
                try {
                    ((arg_impl<int> *) arg_)->set(std::stoi(argv_[i]));
                } catch (std::invalid_argument &e) {
                    throw std::runtime_error(((arg_impl<int> *) arg_)->get_var_name() + " 需要一个 int 类型的参数！");
                }
                break;
            case ARG_STRING:
                ((arg_impl<std::string> *) arg_)->set(argv_[i]);
                break;
        }
    }

    for (auto *pos: m_positions) {
        if (!pos->has_set()) {
            throw std::runtime_error(pos->get_var_name() + " 必须传入值！请使用 -h 或 --help 查看帮助！");
        }
    }
}

void parser::show_help() {
    std::cout << "Usage: " << m_prog;
    for (auto *pos_: m_positions) {
        pos_->show_usage();
    }
    m_helper.show_usage();
    for (auto *opt_: m_options) {
        opt_->show_usage();
    }
    std::cout << std::endl << std::endl << "positional" << std::endl;
    for (auto *pos_: m_positions) {
        pos_->show_help();
    }
    std::cout << std::endl << "optional" << std::endl;
    m_helper.show_help();
    for (auto *opt_: m_options) {
        opt_->show_help();
    }
}

template<typename Val = bool>
void parser::add_argument(const std::string &small_name_, const std::string &full_name_,
                          const std::string &desc_) {
    if (small_name_ == "-h" || full_name_ == "--help") {
        throw std::runtime_error("help 选项参数不能被替换，请选用其他字母和单词");
    }
    argument *new_arg = new arg_impl<Val>(small_name_, full_name_, desc_);
    m_options.emplace_back(new_arg);
}

template<typename Val = void *>
void parser::add_argument(const std::string &arg_name_, const std::string &desc_) {
    if (arg_name_[0] == '-' && arg_name_[1] == '-') {
        if (arg_name_ == "--help") {
            throw std::runtime_error("help 选项参数不能被替换，请选用其他字母和单词");
        }

        argument *new_arg = nullptr;
        if (typeid(Val) != typeid(void *)) {
            new_arg = new arg_impl<Val>("", arg_name_, desc_);
        } else {
            new_arg = new arg_impl<bool>("", arg_name_, desc_);
        }
        m_options.emplace_back(new_arg);
    } else if (arg_name_[0] != '-') {
        argument *new_arg = nullptr;
        if (typeid(Val) != typeid(void *)) {
            new_arg = new arg_impl<Val>("", arg_name_, desc_);
        } else {
            new_arg = new arg_impl<std::string>("", arg_name_, desc_);
        }
        m_positions.emplace_back(new_arg);
    } else {
        throw std::runtime_error("以 --xxxx 的形式传入参数全名");
    }
}

template<typename Val>
Val &parser::get(const std::string &var_name_) {
    int type_ = ARG_BOOL;
    if (typeid(bool) == typeid(Val)) {
        type_ = ARG_BOOL;
    } else if (typeid(int) == typeid(Val)) {
        type_ = ARG_INT;
    } else if (typeid(std::string) == typeid(Val)) {
        type_ = ARG_STRING;
    } else {
        throw std::runtime_error("当前只支持 bool, int, std::string。");
    }

    for (auto *arg_: m_options) {
        if (type_ == arg_->get_type() && arg_->get_var_name() == var_name_) {
            auto *opt = (arg_impl<Val> *) arg_;
            return opt->get();
        }
    }
    for (auto *arg_: m_positions) {
        if (type_ == arg_->get_type() && arg_->get_var_name() == var_name_) {
            auto *opt = (arg_impl<Val> *) arg_;
            return opt->get();
        }
    }
    throw std::runtime_error("没有变量名为 " + var_name_ + " 的参数！");
}

#endif //ARGS_PARSER_PARSER_H
