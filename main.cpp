#include <iostream>
#include "type.h"

int main(int argc, char *argv[]) {
    parser::type t("123.9");
    int a = t;
    float b = t;
    std::string c = t;
    std::cout << a << ", " << b << ", " << c << std::endl;
    return 0;
}