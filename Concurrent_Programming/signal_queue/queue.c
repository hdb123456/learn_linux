#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

struct msgbuf{
	long mtype;
	char mtext[80];
};
void send_msg(int qid, int msgtype)
{
	struct msgbuf msg;
	msg.mtype=msgtype;

	printf("input text:");
	fgets(msg.mtext,sizeof(msg.mtext),stdin);
	if(strlen(msg.mtext)>sizeof(msg.mtext))
	{
		perror("text too long");
		exit(EXIT_FAILURE);
	}
	msg.mtext[strcspn(msg.mtext, "\n")] = '\0';
	//	printf("%s",msg.mtext);
	if(msgsnd(qid,&msg,sizeof(msg.mtext),0)==-1)
	{
		perror("msgsnd error");
		exit(EXIT_FAILURE);
	}
}
void get_msg(int qid, int msgtype)
{
	struct msgbuf msg;
	msg.mtype=msgtype;
	msgrcv(qid,&msg,sizeof(msg.mtext),msg.mtype,0);
	printf("msgtype:%ld msgtext:%s\n",msg.mtype,msg.mtext);
}
int main(int argc, char *argv[])
{
	key_t key=0;
	int qid,msgtype=1;
	if(argc!=2)
	{
		perror("argc error");
		printf("please input mode: \njust like :\nSend Data:./a.out 2\nGet Data:./a.out 1\n");
		exit(EXIT_FAILURE);
	}
	key=ftok(".",100);
	if (key<0){
		perror("ftok");
		exit(EXIT_FAILURE);
	}
	if((qid=msgget(key,IPC_CREAT|0666))<0){
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	if(*argv[1]=='1'){
		printf("Current Mode:Get Data\n");
		get_msg(qid,msgtype);
	}
	else if(*argv[1]=='2'){
		printf("Current Mode:Send Data\n");
	 	send_msg(qid, msgtype);
	 }
}


