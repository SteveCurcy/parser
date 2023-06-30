# parser
[![](https://img.shields.io/badge/Author-Xu.Cao-lightgreen)](https://github.com/SteveCurcy) 
![](https://img.shields.io/badge/Version-0.1.1-yellow)

基于 C++ 的 main 函数参数解析工具

## 目标 🏁

本项目最终希望实现一个开源的、开箱即用的参数解析库。

用户可以不必关心如何获取参数，或者参数出错应该如何处理，而只需将希望获取的参数交给参数解析类。

在整个使用过程中都不会出现模板模式，而是使用其他方式予以代替。

## 设计模式 🎨

本项目将采用 Meyers 单例模式，利用 C++ Magic Static 特性实现。一个进程中只能存在一个用于参数解析的实例。

本项目将遵循“最简化”的设计原则，只提供必要的操作接口，保证尽量少的内部错误。

## 当前版本特性 🎉

目前，本项目仅支持"整数类型、浮点数类型、字符串类型"的参数类型。当赋值的变量类型与参数指定的类型不相同时则会产生报错。

对于参数解析得到的数据，都必须是 Read-Only 的，因为他们是系统或用户传入的，作为值而不能作为变量使用。

参数分为两种，分别是位置参数和可选参数。

- 位置参数：只有一个名称，且不能以短横线开始；
- 可选参数：可以设置短名称和长名称，必须以短横线开始，
    - 短名称：必须以一个短横线开始，如“-h”；
    - 长名称：必须以一对短横线开始，如“--help”

目前只考虑支持 string 和整数类型。

## 使用/示例 🌰

如 main.cpp 中给出的示例，你需要先使用 `get_instance` 来获取一个参数解析实例；

然后，你需要使用 `add_argument` 函数来为实例添加要解析的参数，如果你想添加位置参数，可以将第一个参数传入 nullptr，这表明你想要创建一个位置参数；否则则表示要创建一个可选参数；

在参数创建时，你可以指定参数的类型，如 BOOL 即记录该参数是否出现过，参数类型默认为 string 类型。解析器允许你传入的参数进行适当的类型转换：

- int <-> float
- int/float/bool -> string
- int/float/string -> bool

但是需要注意的是，不要用 `auto` 关键字来接收要获取的参数的值，这会使得解析器不知道你想要它返回一个什么类型的值。

正确的方法应该是，指定数据类型，这样解析器可以判断该参数的值是否可以转换为指定类型，如果不能则会报错；如果可以的话，则会执行隐式类型转换。

此外，你还可以指定参数对应的变量名。如果你直接使用 cout 来输出参数，则会输出参数值的原始内容，而不是你想要的类型。这可能导致意想不到的错误。

具体使用方式，请见 main.cpp。

## 使用许可 :page_facing_up:
[GPL 2.0](./LICENSE) &copy; Xu.Cao (Steve Curcy)
