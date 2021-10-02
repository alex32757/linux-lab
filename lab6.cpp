#include <iostream>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int period;

void handler_stop(int sigsum) {};

void signalFunction(int signum) {    
	pid_t pid = fork();
	if (pid == 0){
        time_t seconds = time(NULL);
		cout << "PID потомка: " << getpid() << endl;
		cout << "Время потомка: " << ctime(&seconds) << "\n";
		exit(0);
	}
    waitpid(pid, NULL, 0);

    period++;
}


int main(int argc, char* argv[]) {
    time_t seconds_start = time(NULL);

    cout << "Старт родителя: " << ctime(&seconds_start) << endl;

    signal(SIGALRM, signalFunction);
    signal(SIGTSTP, handler_stop);

    period = 0;

    itimerval timer;
	timer.it_interval.tv_sec = atoi(argv[1]);
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = atoi(argv[1]);
    timer.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, nullptr);
    
    while (period != atoi(argv[2]));

    time_t seconds = time(NULL);
	cout << "Окончание родителя: " << ctime(&seconds) << endl;
	cout << "Общее время работы родителя: " << seconds - seconds_start << "\n";
    
	return 0;
}