# args_parser
[![](https://img.shields.io/badge/Author-Xu.Cao-lightgreen)](https://github.com/SteveCurcy) 
![](https://img.shields.io/badge/Version-0.0.1-yellow)

基于 C++ 的 main 函数参数解析工具

## 内容列表

- [1. 背景 :cookie:](#1-背景-cookie)
- [2. 安装和运行 :candy:](#2-安装和运行-candy)
- [3. 示例 :birthday:](#3-示例-birthday)
- [4. 现有问题 :sandwich:](#4-现有问题-sandwich)
- [5. 使用许可 :page_facing_up:](#5-使用许可-page_facing_up)

## 1. 背景 :cookie:
当前 C++ 并没有提供一个参数解析库，因此，涉及参数解析时往往需要开发者自行编写或使用开源项目。
本项目的目的也是提供一个简单易用的参数解析工具。

## 2. 安装和运行 :candy:
使用本项目只需将 parser 头文件引入即可，类似一个参数解析库。

## 3. 示例 :birthday:
你可以很简单的使用 parser，类似 main.cpp 中的示例代码，目前你只能通过 add_argument 来添加一个可选参数。

当前支持的数据类型包括 `bool, int, std::string`。你可以通过 `add_argument<>([短名字],全名,描述)` 
增加一个参数，然后通过 `get<Val>(var_name);` 来获取参数的值，请确保你定义添加的参数和获取的变量值类型相同，
否则将出现错误。

此外，当前版本暂时使用全名来作为变量名，如 "--disk" 的参数名则应该使用变量名为 "disk"。此外，请不要添加帮助
参数，这是不允许的，是内置在解析器中的。

现在已经开始支持位置参数，即不以 "-" 或者 "--" 开头的参数。如 `xxx.out number 123`，变量名则与参数名相同。
位置参数是必须传入的，否则将报错，这也是位置参数与可选参数的区别之一。

```c++
#include "parser.h"

int main(int argc, char *argv[]) {
    auto &parser = parser::get_instance();
    parser.add_argument<std::string>("--disk", "no fucking help");
    parser.parse(argc, argv, "args_parser_test");

    auto &disk_ = parser.get<std::string>("disk");
    std::cout << disk_ << std::endl;
    return 0;
}
```

## 4. 现有问题 :sandwich:
不支持中文，由于编码问题，中文的加入将使得不指定编码的参数解析无法进行，因此暂时无法支持中文。

## 5. 使用许可 :page_facing_up:
[GPL 2.0](./LICENSE) &copy; Xu.Cao (Steve Curcy)
