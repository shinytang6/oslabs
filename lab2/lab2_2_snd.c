// msgsnd.c
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
	int qid = msgget(1234, IPC_CREAT|0666);
	if(qid < 0) {
		printf("Creating message queue failedi.\n");
		return -1;
	}
	printf("qid = %d\n", qid);
	buf.type = 1;
	strcpy(buf.text, "Hello msgrcv!\n");

	int flag = msgsnd(qid, &buf, sizeof(buf.text), 0);
	if(flag < 0) {
		printf("Sending message failed\n");
		return -1;
	}

	msgrcv(qid, &buf, sizeof(buf.text), 1, 0);
	printf("Received message type = %d, text = %s.", buf.type, buf.text);

	system("ipcs -q");

}
