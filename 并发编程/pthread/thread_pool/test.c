#include <stdio.h>

void print(void *arg)
{
printf("arg=[%d]\n",*(int*)arg);

}

int main()
{

	int i=0;
	print((void*)&i);

}
