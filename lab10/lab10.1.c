#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>

//0 - мьютекс файла 
//1 - семафор numProcess
//2 - семафор numWriters
//3 - семафор numReaders


struct sembuf writer_dec_sem = {0, -1, 0};
struct sembuf writer_inc_sem = {0, 1, 0};

struct sembuf numwriters_dec = {2, -1 , 0};
struct sembuf numwriters_inc = {2, 1 , 0};

struct sembuf numreaders_dec = {3, -1 , 0};
struct sembuf numreaders_inc = {3, 1 , 0};

struct sembuf is_reader_zero = {3, 0, 0};

struct sembuf inc_proces = {1, 1, 0};
struct sembuf dec_proces = {1, -1, 0};

FILE *file;
int main(int argc, char **argv){
	
	int semId;
	int writeNum = atoi(argv[1]);

	int pid = getpid();
	struct sembuf sem_init = {0, 1, 0};

	key_t key = 11;
	semId = semget(key, 4, IPC_CREAT | IPC_EXCL | 0666);

	if(semId > 0) {
		file = fopen("output.txt", "w");
		fclose(file); //очистка файла
		printf("Процесс №%d создал новый семафор\n", pid);
		semop(semId, &sem_init, 1); //мьютекс файла	
	} else {
		semId = semget(key, 4, IPC_EXCL);
		printf("Используется существующий семафор: %d\n", semId);
	}
	semop(semId, &inc_proces, 1); //общее количество процессов	


	printf("semid: %d\n", semId);
	
	for(int i = 0; i < writeNum; i++){
		semop(semId, &numwriters_inc, 1);

		semop(semId, &is_reader_zero, 1);
		
		printf("Процесс-писатель ожидает освобождения writer mutex\n");
		semop(semId, &writer_dec_sem, 1);
		printf("Процесс %d записал %d из %d строк\n", pid, i + 1, writeNum);
		file = fopen("output.txt", "a");
		fprintf(file,"Процесс-писатель %d  %d of %d\n", pid, i + 1, writeNum);
		fclose(file);
		sleep(2);
		printf("Процесс-писатель освободил writer mutex\n\n");
		semop(semId, &writer_inc_sem, 1);

		semop(semId, &numwriters_dec, 1);
		sleep(1);
	}
	
	semop(semId, &dec_proces, 1);
	if(semctl( semId, 1, GETVAL, 0 ) == 0){
		semctl(semId, IPC_RMID, 0);
		printf("Семафор уничтожен\n");
	}

	return 0;
}