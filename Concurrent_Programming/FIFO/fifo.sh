#!/bin/bash

PIPE_FILE="fifo"
#check whether if pipe file
if [ -e "$PIPE_FILE" ]
	then
			echo "Pipe file already exists"
else
			echo "Creating pipe file..."
			mkfifo "$PIPE_FILE"
fi

#if gcc -o fifo_r.exe fifo_r.c
#	then
#			echo "Compilation for fifo_r.c successful."
#	else
#			echo "Compilation failed."
#fi

#if gcc -o fifo_w.exe fifo_w.c
#	then
#			echo "Compilation for fifo_r.c successful."
#	else
#			echo "Compilation failed."
#fi


