PIPE_FILE="fifo"

if gcc -o fifo_w.exe fifo_w.c
  then
      echo "Compilation for fifo_r.c successful."
  else
      echo "Compilation failed."
fi
./fifo_w.exe $PIPE_FILE
