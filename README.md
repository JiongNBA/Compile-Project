# Compile-Project: FLYCompiler

This project is for ZJU 2022 Spring-Summer Compilation Principle course, contributed by Yang Haofeng, Wang Yuesong and Li Jiong.
## 1.1 项目简介

本项目实现了 `flySCC`（`fly Subset-of-C Compiler`），一个`C`语言子集编译器。`flySCC`支持读入项目自定义的.fly后缀文件，可以选择生成`IR`后直接运行；也能够支持生成`IR`和目标代码。

本项目主体使用 `c++17` 标准实现，基于`Flex`，`Bison` 以及 `LLVM` 等工具库完成开发。此外，本项目还使用了 `d3.js` 来可视化 `AST` 。

项目的技术路线为：

- `Flex`对输入源代码文件进⾏词法分析；
- `Bison`调用`Flex`提供的yylex接口，获取token进行语法、语义分析获得源代码对应的抽象语法树AST；
- 使用`LLVM`提供的生成`IR`的API，遍历语法树生成平台无关的中间表示`IR`；
- 使用 `clang` 编译 `IR` 文件，生成目标代码。

编译器支持的语法和基本类型为：

- 数据类型
  - int (32-bit)
  - char (8-bit)
  - float (64-bit)
  - void
  - literal（字符串）
- 语法
  - 函数的定义与调用；
  - 变量的定义、初始化、赋值与引用（全局、局部）；
  - 多维数组的定义、默认初始化、赋值与引用；
  - 控制流语句（if-elif-else, while, for, break, continue）；
  - 多种运算符（单目: !,-,&; 双目: +-*/,&&,||,>,<,>=,<=,==,!=,=）；
  - 注释（单行注释// 和多行注释 /* */）；
  - 以main函数为入口；
  - 拓展式表达式（连续取非，表达式列表等）

## 1.2 开发环境

| 操作系统               | Ubuntu 20.04      |
| ---------------------- | ----------------- |
| 编译环境               | Flex    2.6.4  <br>  Bison 3.5.1(GNU Bison) <br> LLVM 10.0.0  <br>  clang 10.0.0|
| 编辑器                 | Visual Stdio Code |

## 1.3 项目结构说明

```
.
├── bin             --> 生成的目标代码文件夹
│   ├── flySCC         --> 编译器文件
│   ├── test1.o        --> 测试一可执行文件
│   ├── test2.o        --> 测试二可执行文件
│   └── test3.o        --> 测试三可执行文件
├── clean.sh        --> 批量清理脚本文件
├── include         --> 头文件文件夹
│   ├── AST.h
│   ├── IO.h
│   ├── semantic.h
│   └── SymTable.h
├── Makefile
├── run.sh          --> 批量测试脚本文件
├── src             --> 源代码文件文件夹
│   ├── AST.cpp         --> AST结点及生成IR文件
│   ├── flySCC.cpp      --> 项目代码顶层文件
│   ├── IO.cpp          --> IO函数定义文件
│   ├── lexer.l         --> 词法分析文件
│   ├── Makefile
│   ├── parser.ypp      --> 语法分析文件
│   ├── semantic.cpp    --> 语义分析接口
│   └── SymTable.cpp    --> 符号表源文件
├── test            --> 测试代码文件夹
│   ├── test1.fly       --> 测试代码文件一
│   ├── test2.fly       --> 测试代码文件二
│   ├── test3.fly       --> 测试代码文件三

```

## 1.4 小组分工

| 组员   | 成员分工                                                 |
| ------ | -------------------------------------------------------- |
| 杨浩峰 | 词法分析，语法分析，语义分析，中间代码生成，目标代码生成 |
| 王越嵩 | 词法分析，语法分析，语义分析，中间代码生成，目标代码生成 |
| 李炯   | 词法分析，语法分析，语义分析，中间代码生成，目标代码生成 |

注：由于小组成员一致认为本项目不便划分，因此小组成员都参与了每一个环节。
