# 数值分析平台前端

## 前置条件
- 系统：Ubuntu 18
- 编译器：C++ 11
- 安装Rose Compiler：<http://rosecompiler.org/>
- 安装Boost库
- 安装cmake

## 使用方法
- 修改CMakeLists.txt第6、7行的Rose Compiler路径
- 修改analysis.sh第2行为IR文件输出路径的绝对地址
- 使用cmake build项目
- 执行analysis.sh，根据控制台提示输入待分析C++项目的绝对地址

## TODO
- [x] 分段
    - [x] 循环分段
    - [x] 函数分段
    - [x] 输入、输出变量
- [x] 循环
    - [x] for循环
        - [x] 单层for循环
        - [x] 嵌套for循环
    - [x] while循环
    - [x] do...while循环
- [x] 递归
    - [x] 检测函数调用
    - [x] 处理递归
- [x] 矩阵
    - [x] 矩阵变量表示
    - [x] 定位矩阵运算
    - [x] 由内向外分析下标信息
- [ ] IR文件
    - [x] 导入JSON库
    - [ ] 确认IR文件格式
    - [x] 生成IR文件
    - [x] 生成IR内容

## IR文件格式沟通列表
- 一个源文件含有多个function
- 一个function含有多个return值

## 问题列表
- ++、+=、-符号支持
- 输入变量过多
- IR文件格式确认
- 矩阵下标分析待复查（找bug）
- 初步生成IR文件（待找bug）