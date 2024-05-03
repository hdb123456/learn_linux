#!/bin/bash
read -p "Enter the filename: " FILE

if [ -e $FILE ]
	then
		echo "$FILE already exists"
else
		echo "Creating $FILE file..."
		fallocate -l 1K $FILE
fi


gcc mmap.c -o mmap

./mmap $FILE
