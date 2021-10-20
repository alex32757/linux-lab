#include <iostream>
#include <sys/msg.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

int message_pid;

struct MessageStruct {
    long type;   
    int text;       
};

void signalFunction(int signum) {
	msgctl(message_pid, IPC_RMID, nullptr);
	exit(0);
}

int main(int argc, char *argv[]) {
	int wait_time = atoi(argv[1]);

	MessageStruct message;

	signal(SIGALRM, signalFunction);

	message_pid = msgget(11, (IPC_CREAT | 400 | 2)); //создание очереди
	if(message_pid == -1){
		cout << "Невозможно создать очередь" << endl;
		return 0;
	}

	alarm(wait_time);
    
	while(true) {
	    if( msgrcv(message_pid, &message, sizeof(MessageStruct), 1, IPC_NOWAIT) != -1){
	    	cout << "Полученное сообщение: " << message.text << endl;
	    	wait_time = message.text;
			alarm(wait_time);
	    } 
	}

    msgctl(message_pid, IPC_RMID, nullptr);

	return 0;
}