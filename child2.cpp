#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <signal.h>
#include <wait.h>

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
	sigaddset(&mask, SIGUSR2);
	sigprocmask(SIG_BLOCK, &mask, nullptr);

	signal(SIGINT, signal_handler);
	
	sigwait(&mask, &signum);
	while((bytes = read(*argv[2], &c, 1)) || flag) {
		// if (bytes == 0)
		// 	break;
		cout << "P2 ch: " << c << endl;
		out << c;
		kill(0, SIGUSR1);
		sigwait(&mask, &signum);		
	}
	kill(0, SIGUSR1);
	out.close();
	exit(EXIT_SUCCESS);
	return 0;
}