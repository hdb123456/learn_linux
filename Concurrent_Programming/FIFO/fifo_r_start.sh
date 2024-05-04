PIPE_FILE="fifo"

if gcc -o fifo_r.exe fifo_r.c
    then
        echo "Compilation for fifo_r.c successful."
    else
        echo "Compilation failed."
fi
./fifo_r.exe $PIPE_FILE

