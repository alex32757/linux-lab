#include <iostream>
#include <fstream>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>

int pipe1;
std::ofstream file;

void handler_usr(int signum) {
    char c;
    read(pipe1, &c, 1);
    file << c;
    kill(getppid(), signum);
}

void handler_quit(int signum) {
    exit(0);
}

int main(int argc, char* argv[]) {
    file.open(argv[1]);
    pipe1 = atoi(argv[2]);
    int signum = atoi(argv[3]);

    signal(signum, handler_usr);
    signal(SIGQUIT, handler_quit);
    kill(getppid(), signum);

    while (true);
    return 0;
}
