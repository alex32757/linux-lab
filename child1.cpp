#include <iostream>
#include <fstream>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>

int pipe1;
std::ofstream file;

void handler_quit(int sig) {
    exit(0);
}

void handler_usr(int sig) {
    char c;
    read(pipe1, &c, 1);
    file << c;
    kill(getppid(), sig);
}

int main(int argc, char* argv[]) {
    file.open(argv[1]);
    pipe1 = atoi(argv[2]);
    int sig = atoi(argv[3]);

    signal(sig, handler_usr);
    signal(SIGQUIT, handler_quit);
    kill(getppid(), sig);

    while (true);
    return 0;
}
