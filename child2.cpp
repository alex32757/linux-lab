#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>

using namespace std;

bool flag = true;

void signal_handler(int signum) {
	cout << " Quit2" << endl;
	flag = false;
}

int main(int argc, char** argv) {
	ofstream out(argv[1]);
	int signum, bytes;
	char c;
	
	cout << "P2 start" << endl;
	sigset_t mask;
	sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_SETMASK, &mask, NULL);
	sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
	signal(SIGQUIT, signal_handler);
	
	sigwait(&mask, &signum);
	//while((bytes = read(*argv[2], &c, 1)) || flag) {
	while(true) {
		if (read(*argv[2], &c, 1)) {
			cout << "P2 ch: " << c << endl;
			out << c;
			kill(0, SIGUSR1);
			sigwait(&mask, &signum);
		} else {
			if (flag == false) break;
		}	
	}
	kill(0, SIGUSR1);
	out.close();

	return 0;
}