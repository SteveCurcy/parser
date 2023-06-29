#include <iostream>
#include "args.h"

int main(int argc, char *argv[]) {
    auto arg = parser::args("help", "show this help text", parser::TYPE_INT);
    std::cout << int(arg.get()) << std::endl;
    arg.show_help();
    return 0;
}