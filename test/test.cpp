#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <atomic>

std::atomic<int> m_write;

void *thread_func(void *arg) {
    while(m_write) {
        std::cout << "thread work" << std::endl;
        usleep(10000);
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    pthread_t th1;
    m_write = 1;
    sleep(1);

    pthread_create(&th1, nullptr, thread_func, argv[1]);
    sleep(2);

    std::cout << "signal cathc" << std::endl; 
    m_write = 0;


    return 0;
}
 