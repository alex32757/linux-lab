#include <signal.h>
#include <iostream>

using namespace std;

void SigfpeError(int signum) {
    cout << "SIGFPE error (Invalid operation (overflow, division by 0))" << endl;
    exit(1);
}

void SigsegvError(int signum) {
    cout << "SIGSEGV error (Memory protection violation)" << endl;
    exit(2);
}

int main(int argc, char* argv[]) { //1 - SIGFPE, 2 - SIGSEGV
    signal(SIGFPE, SigfpeError);
    signal(SIGSEGV, SigsegvError);

    switch (atoi(argv[1]))
    {
    case 1:
        int res;
        res = 1 / 0;
        break;
    
    case 2:
        char* p;
        *p = 'a';
        break;
    
    default:
        cout << "Invalid parameters" << endl;
    }

    return 0;
}