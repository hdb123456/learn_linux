#!/bin/bash

# 提示用户输入文件名（可以包括路径），并将输入存储在变量 'file' 中
read -p "input a file name (with path if needed): " file

# 输出用户输入的文件名
echo "You entered: $file"

# 检查文件是否存在
if [ -e "$file" ]; then
    echo "File '$file' exists."
else
    echo "File '$file' does not exist."
fi
