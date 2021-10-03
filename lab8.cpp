#include <iostream>
#include <sys/msg.h>

using namespace std;

struct MessageStruct {
    long type;   
    int text;       
};

int main(int argc, char *argv[]) {
	int wait_time = atoi(argv[1]);

	MessageStruct message;

	int message_pid = msgget(11, (IPC_CREAT | 400 | 2));
	if(message_pid == -1){
		cout << "Невозможно создать очередь" << endl;
		return 0;
	}

	time_t start_time = time(NULL);
    
	while(time(NULL) - start_time < wait_time) {
	    if( msgrcv(message_pid, &message, sizeof(MessageStruct), 1, IPC_NOWAIT) != -1){
	    	cout << "Полученное сообщение: " << message.text << endl;
	    	start_time = time(NULL);
	    	wait_time = message.text;
	    } 
	}

    msgctl(message_pid, IPC_RMID, nullptr);

	return 0;
}