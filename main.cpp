#include <iostream>
#include "parser.h"

int main(int argc, char *argv[]) {
    auto p = parser::parser::get_instance(argc, argv);
    // 设置一个位置参数，并设置参数对应变量的名称
    p.add_argument(nullptr, "goddess", "testing goddess", parser::TYPE_INT, "god");
    // 设置一个可选的 bool 型参数
    p.add_argument("-t", "--test", "testing option", parser::TYPE_BOOL);
    p.parse();
    // 支持进行隐式类型转换
    float fresult = p["god"];
    int iresult = p["god"];
    bool bresult = p["god"];
    std::string sresult = p["test"];
    std::cout << p["test"] << std::endl;
    std::cout << fresult << ", " << iresult << ", " << bresult << ", " << sresult << std::endl;
    return 0;
}