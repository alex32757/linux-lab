#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <signal.h>
#include <wait.h>

using namespace std;

int main(int argc, char* argv[]) {
	ifstream file1(argv[1]);

	int pipe_d[2];
	pid_t pid1, pid2;
	string buf;

	sigset_t new_mask;
	sigaddset(&new_mask, SIGUSR1);
	sigaddset(&new_mask, SIGUSR2);
	sigaddset(&new_mask, SIGQUIT);
	sigprocmask(SIG_SETMASK, &new_mask, NULL);

	pipe(pipe_d);

	pid1 = fork();
	if (pid1 == 0) {
		// Child1 
		close(pipe_d[1]);
		execl("child1", "child1", argv[2], &pipe_d[0], NULL);
	}
	else {
		pid2 = fork();
		if (pid2 == 0) {
			// Child2 
			close(pipe_d[1]);
			execl("child2", "child2", argv[3], &pipe_d[0], NULL);
		}
		else {
			// Parent 
			close(pipe_d[0]);
			sleep(1);
			while(getline(file1, buf)){
				//sleep(1);
				write(pipe_d[1], &buf[0], strlen(buf.c_str()));;
				cout << "Parent write symbol:" <<  &buf[0] << endl;
				
			}
			
			kill(pid1, SIGINT);
			kill(pid2, SIGINT);
			cout << "Parent end writing" << endl;
			file1.close();
			
			close(pipe_d[1]);
			waitpid(pid1, nullptr, 0);
			waitpid(pid2, nullptr, 0);
		}
	}

	return 0;
}