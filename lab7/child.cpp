#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char *argv[]) {
    ofstream output;

    output.open(argv[1], output.out | output.app); 
    
    sleep(atoi(argv[2]));
    
    pid_t pid = getpid();
    output<< argv[3] << ": Идентификатор процесса --" << getpid() << endl;
    output<< argv[3] << ": Идентификатор предка --" << getppid() << endl;
    output<< argv[3] << ": Идентификатор сессии процесса --" << getsid(pid) << endl;
    output<< argv[3] << ": Идентификатор группы процессов --" << getpgid(pid) << endl;
    output<< argv[3] << ": Реальный идентификатор пользователя --" << getuid() << endl;
    output<< argv[3] << ": Эффективный идентификатор пользователя --" << geteuid() << endl;
    output<< argv[3] << ": Реальный групповой идентификатор  --" << getgid() << endl;
    output<< argv[3] << ": Эффективный групповой идентификатор --" << getegid() << endl << endl;
    
    output.close();
    
    return 0;
}