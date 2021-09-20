#include <pthread.h>
#include <iostream>
#include <fstream>

using namespace std;

ifstream source("./source.txt");
ofstream output1("./output1.txt");
ofstream output2("./output2.txt");

void *whriteInOutput1(void *arg) {
    string str = *static_cast<string*>(arg);
    output1 << str << endl;
    pthread_exit(0);
}

void *whriteInOutput2(void *arg) {
    string str = *static_cast<string*>(arg);
    output2 << str << endl;
    pthread_exit(0);
}

int main() {
    string str1, str2;
    pthread_t th1, th2;
    bool flag1 = true, 
         flag2 = true;

    while (flag1 && flag2)
    {
        if (getline(source, str1)) 
            pthread_create(&th1, nullptr, whriteInOutput1, &str1);
        else 
            flag1 = false;
    
        if (getline(source, str2)) 
            pthread_create(&th2, nullptr, whriteInOutput2, &str2);
        else 
            flag2 = false;

        pthread_join(th1, nullptr);
        pthread_join(th2, nullptr);
    }

    source.close();
    output1.close();
    output2.close();
    
    return 0;
}