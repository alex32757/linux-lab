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

	int message_pid = msgget(11, IPC_CREAT|IPC_EXCL);

	if(message_pid != -1){
		cout << "Вторая программа не запущена" << endl;
		msgctl(message_pid, IPC_RMID, 0);
		return 0;
	}

	message_pid = msgget(11, IPC_CREAT);

    message.type = 1;
    message.text = wait_time;

    if( (msgsnd(message_pid, &message, sizeof(MessageStruct), 0)) != -1) 
    	cout << "Отправленное сообщение: " <<  message.text << endl;
    else
    	cout << "Ошибка отправки сообщения" << endl;
    
    return 0;
}