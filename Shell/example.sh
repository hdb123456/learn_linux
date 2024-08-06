#!/bin/bash

# 打印"hello world"，echo 自带换行，对 '', "" 无区别
echo "hello world"

# 变量初始化
COUNT=3 # 变量无需声明，等号前后不能带空格

# 强、弱引用
echo $COUNT # 输出变量等价于 echo "$COUNT" (弱引用)
echo '$COUNT' # 强引用，打印 $COUNT

# 删除变量
unset COUNT

# 等效
A=`echo $COUNT`
A=$(echo $COUNT)

# 数组
a=(1 2 3 4 5)
echo $a # 打印第一个元素，结果是1

# 打印数组中的所有元素
echo ${a[*]}
echo ${a[@]}

# 取数组元素个数
echo ${#a[*]}
echo ${#a[@]}

# 取出一个元素
echo ${a[2]} # 打印第3个元素，结果是3

# 数组赋值
a[2]=9

# 删除数组或数组元素
unset a # 删除整个数组
unset a[2] # 删除数组中第3个元素

# 打印数组中从下标1开始的3个元素
echo ${a[*]:1:3} 

# 只读变量，不能修改值
readonly COUNT=3

# 位置参数变量
echo $0 # 脚本名
echo $1 # 第一个参数
echo $2 # 第二个参数
echo $3 # 第三个参数
echo $# # 参数数量，不包括脚本名

# 运行脚本并传递参数，例如：./example.sh 4 5 6 8 9
# 打印结果：
# ./example.sh
# 4 
# 5 
# 6
# 5
# 4 5 6 8 9
# 4 5 6 8 9

# 上一个命令的执行结果
echo $?

# 打印当前脚本的进程号
echo $$

# 常用的环境变量
echo $USER # 当前用户
echo $HOME # 用户的宿主目录
echo $LANG # 语言和字符集
echo $PWD  # 当前工作目录
echo $PATH # 可执行用户程序的默认路径

# export 将变量指定为全局变量