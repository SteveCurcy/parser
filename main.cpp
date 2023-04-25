#include "parser.h"

int main(int argc, char *argv[]) {
    auto &parser = parser::get_instance();
    parser.add_argument<int>("--disk", "no fucking help");
    parser.parse(argc, argv, "args_parser_test");

    auto &disk_ = parser.get<int>("disk");
    std::cout << disk_ << std::endl;
    return 0;
}