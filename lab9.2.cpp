#include <iostream>
#include <sys/types.h>
#include <sys/shm.h>
#define ARR_SIZE 10 // Кол-во элементов в массиве

using namespace std;

struct shr_struct {
    int arr[ARR_SIZE];
    int lock; 
}; 

int main(int argc, char* argv[]) {
    void* adr1 = nullptr, 
        * adr2 = nullptr; // Указатель на виртуальный адрес
    int shmid1, shmid2;
    int count = atoi(argv[1]);
 
    // Запросы на разделяемые сегменты памяти
    shmid1 = shmget((key_t)1, sizeof(shr_struct), (0666 | IPC_CREAT)); 
    shmid2 = shmget((key_t)2, sizeof(shr_struct), (0666 | IPC_CREAT));

    // Включение разделяемой памяти в адресное пространство процесса
    adr1 = shmat(shmid1, nullptr, 0);
    adr2 = shmat(shmid2, nullptr, 0);
    shr_struct* shr1 = (shr_struct*)adr1;
    shr_struct* shr2 = (shr_struct*)adr2;

    for (int i = 0; i < count; i++) {
        while (shr1->lock == 1 && shr2->lock == 0);
        cout << "P2: Процесс вошел в критическую секцию" << endl;
        cout << "Массив, полученный из первой программы: ";
        for (int i = 0; i < ARR_SIZE; i++) {
            cout << shr1->arr[i] << " ";
            shr2->arr[i] = shr1->arr[i] - 1;
        }
        cout << endl;

        shr1->lock = 1;
        shr2->lock = 0;
        cout << "P2: Процесс вышел из критической секции" << endl;
    }

    shmdt(adr2);
    shmctl(shmid2, IPC_RMID, nullptr);

    return 0;

}