#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void writeFile(string processName, std::ofstream& output) {
    pid_t pid = getpid();
    output<< processName << ": Идентификатор процесса --" << getpid() << endl;
    output<< processName << ": Идентификатор предка --" << getppid() << endl;
    output<< processName << ": Идентификатор сессии процесса --" << getsid(pid) << endl;
    output<< processName << ": Идентификатор группы процессов --" << getpgid(pid) << endl;
    output<< processName << ": Реальный идентификатор пользователя --" << getuid() << endl;
    output<< processName << ": Эффективный идентификатор пользователя --" << geteuid() << endl;
    output<< processName << ": Реальный групповой идентификатор  --" << getgid() << endl;
    output<< processName << ": Эффективный групповой идентификатор --" << getegid() << endl << endl;
}

int main(int argc, char* argv[]) {
    char out_file_name[30];
    pid_t pid_child1, pid_child2; 
    ofstream output;
    
    cout << "Введите название файла: ";
    cin >> out_file_name;
    output.open(out_file_name, output.out | output.app); //открытие на запись в конец файла

    output << "Задержки: Parent: " << argv[1] << ", Child 1: " << argv[2] << ", Child 2: " << argv[3] << endl << endl;

    pid_child1 = fork();
    if(pid_child1 == 0) {
        sleep(atoi(argv[2]));
        writeFile("Child 1", output);
    }
    else {
        pid_child2 = vfork();
        if(pid_child2 == 0) {
            execl("child.out", " ", out_file_name, argv[3], "Child 2", NULL);
        } 
        else {
            sleep(atoi(argv[1]));
            writeFile("Parent", output);
            waitpid(pid_child1, nullptr, 0);
            waitpid(pid_child2, nullptr, 0);
        }
    }

    output.close();

    return 0;
}