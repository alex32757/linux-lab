#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>

bool pid1_ready = false;
bool pid2_ready = false;

void handler_usr1(int sig) {
    pid1_ready = true;
}

void handler_usr2(int sig) {
    pid2_ready = true;
}

using namespace std;

int main(int argc, char* argv[]) {
    ifstream file(argv[1]);
    int pipe1[2];
    pipe(pipe1);

    string pipe1s = to_string(pipe1[0]);
    string psig1s = to_string(SIGUSR1);
    string psig2s = to_string(SIGUSR2);

    signal(SIGUSR1, handler_usr1);
    signal(SIGUSR2, handler_usr2);

    pid_t pid1 = fork();
    if (pid1 == 0)
        execl("./child", "./child", argv[2], (char*)pipe1s.data(), (char*)psig1s.data(), NULL);
    
    pid_t pid2 = fork();
    if (pid2 == 0)
        execl("./child", "./child", argv[3], (char*)pipe1s.data(), (char*)psig2s.data(), NULL);

    while (!(pid1_ready && pid2_ready));
    while (!file.eof()) {
        char c;
        if ((c = file.get()) != EOF) { 
            write(pipe1[1], &c, 1);
            pid1_ready = false;
            kill(pid1, SIGUSR1);
            while (!(pid1_ready));
        }

        if ((c = file.get()) != EOF) {
            write(pipe1[1], &c, 1);
            pid2_ready = false;
            kill(pid2, SIGUSR2);
            while (!(pid2_ready));
        }
    }

    kill(pid1, SIGQUIT);
    kill(pid2, SIGQUIT);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
