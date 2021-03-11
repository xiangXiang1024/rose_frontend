# 数值分析平台前端

## 前置条件
- 系统：Ubuntu 18
- 安装Rose Compiler：http://rosecompiler.org/
- 安装Boost库
- 安装cmake

## 使用方法
- 修改CMakeLists.txt第6、7行的Rose Compiler路径
- 修改analysis.sh第2行为IR文件输出路径的绝对地址
- 使用cmake build项目
- 执行analysis.sh，根据控制台提示输入待分析C++项目的绝对地址

## TODO
- [ ] 分段
    - [x] 循环分段
    - [ ] 函数分段
- [ ] 循环
    - [x] for循环
    - [ ] while循环
    - [ ] do...while循环
- [ ] 递归
- [ ] 矩阵