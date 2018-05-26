// msgrcv.c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define TEXT_SIZE 256

struct msgbuf
{
	long type;
	char text[TEXT_SIZE];
};

void main() {
	struct msgbuf buf;
	int qid = msgget(1234, 0);
	if(qid < 0) {
		printf("Getting message queue failed\n");
		return -1;
	}
	printf("qid = %d\n", qid);
	int flag = msgrcv(qid, &buf, sizeof(buf.text), 1, 0);
	if(flag < 0) {
		printf("Receiving message failed\n");
		return -1;		
	}
	while(flag) {
		printf("Received message type = %d, text = %s\n", buf.type, buf.text);
		
		strcpy(buf.text, "Reply: Hello msgsnd!\n");
		msgsnd(qid, &buf, sizeof(buf.text), 0);
		system("ipcs -q");
		return 1;	
	}
}
