#include <iostream>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int period;

void handlerStop(int sigsum) {};

void signalFunction(int signum) {    
	pid_t pid = fork();
	if (pid == 0){
        time_t seconds = time(NULL);
		cout << "PID потомка: " << getpid() << endl;
		cout << "Время потомка: " << ctime(&seconds) << "\n";
		exit(0);
	}
    waitpid(pid, NULL, 0);

    if (--period == 0) {
        time_t seconds = time(NULL);
        cout << "Время оконачния работы родителя: " << ctime(&seconds);
        exit(0);
        
    }
}

int main(int argc, char* argv[]) {
    time_t seconds_start = time(NULL);

    cout << "Старт родителя: " << ctime(&seconds_start) << endl;

    signal(SIGALRM, signalFunction);
    signal(SIGTSTP, handlerStop);

    period = atoi(argv[2]);

    itimerval timer;
	timer.it_interval.tv_sec = atoi(argv[1]);
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = atoi(argv[1]);
    timer.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, nullptr);
    
    for(;;) 
        pause();
    
    time_t seconds = time(NULL);
	cout << "Окончание родителя: " << ctime(&seconds) << endl;
	cout << "Общее время работы родителя: " << seconds - seconds_start << " секунды\n";
    
	return 0;
}