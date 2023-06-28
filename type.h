#ifndef PARSER_TYPE_H
#define PARSER_TYPE_H

#include <cassert>
#include <cstdlib>
#include <string>

namespace parser {
    class type {
    private:
        const char *m_value = nullptr;
    public:
        explicit type(const char *_value): m_value(_value) {}

        operator int() {
            assert(m_value != nullptr);
            char *tmp;
            return (int)strtol(m_value, &tmp, 10);
        }

        operator float() {
            assert(m_value != nullptr);
            char *tmp;
            return strtof(m_value, &tmp);
        }

        operator std::string() {
            assert(m_value != nullptr);
            return std::string(m_value);
        }
    };
}

#endif //PARSER_TYPE_H
