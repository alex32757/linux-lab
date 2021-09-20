#include <iostream>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

void signalFunction(int signum) {
    time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);

	pid_t pid = fork();
	if (pid == 0){
		cout << "PID потомка: " << getpid() << endl;
		cout << "Время потомка: " << asctime(timeinfo) << "\n";
		exit(EXIT_SUCCESS);
	}
}


int main(int argc, char* argv[]) {

   // signal(SIGALRM, signalFunction);
    struct sigaction sa;
    sa.sa_handler = signalFunction;
    sigaction(SIGALRM, &sa, nullptr);

    itimerval timer;
	timer.it_interval.tv_sec = atoi(argv[1]);
    timer.it_value.tv_sec = atoi(argv[1]);

    for (int i = 0; i < atoi(argv[2]); i++) {
        cout << "1" << endl; 
        setitimer(ITIMER_REAL, &timer, nullptr);
       // alarm(atoi(argv[1]));
	//	pause();
    }

    time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	cout << "Время родителя: " << asctime(timeinfo) << "\n";
	
	return 0;
}