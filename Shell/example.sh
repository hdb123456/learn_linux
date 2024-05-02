#!/bin/bash

echo "hello world"#echo自带换行
COUNT=3#变量无需声明；等号前后不能带空格

echo $COUNT#输出变量等价于echo "$COUTN"(弱引用)
echo "$COUNT" #强引用，打印$COUNT

unset COUNT#删除变量

#效果相同
A=`echo $COUNT`
A=$(echo $COUNT)

#数组
a=(1 2 3 4 5)
echo $a #打印1
#打印12345
echo ${a[*]}
echo ${a[@]}
#取元素个数
echo ${#a[*]}
echo ${#a[@]}

#取出一个元素
echo ${a[2]}#打印3
#数组赋值
a[2]=9
#删除
unset a
unset a[2]
#打印
echo ${a[*]:1:3}#打印下标1-3的数组中的数



readonly COUNT=3#只读变量，不能修改值

#位置变量
echo $0
echo $1
echo $2
echo $3
echo $# #打印数量，./example.sh 4 5 6 8 9，其中./example.sh不算在内
echo $*
echo $@
./example.sh 4 5 6 8 9
#打印
./example.sh
4 
5 
6
5
4 5 6 8 9
4 5 6 8 9

echo $?#上一个命令的执行结果
echo $$#打印进程号

#常用的环境变量
$USER
$HOME用户的宿主目录
$LANG语言和字符集
$PWD
$PATH可执行用户程序的默认路径
export 将变量指定为全局变量

read -p "input a file name :" file
#test -e $file
#echo $?
#if test -e $file
if [ -e $file ]
then
    echo ""
else
    echo ""
fi
