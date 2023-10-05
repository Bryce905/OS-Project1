//Bryce Henderson
//Hend0188


#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define SHMKEY ((key_t) 1497)

typedef struct {
    int value;
}
shared_mem;
shared_mem *total;

void process(int process, int goal){      //process to be run by each of the child to increment
    for (int i = 0; i < goal; ++i) {
        total->value++;
    }
    printf("From Process %d: total = %d \n", process+1, total->value);
    exit(1);
}
int main(){
    int shmid, ID, status;
    char *shmadd;
    shmadd = (char *)0;
    if ((shmid = shmget(SHMKEY, sizeof(int), IPC_CREAT | 0666)) < 0){ //declaring the shmid
        perror("shmget");
        exit(1);
    }
    if ((total = (shared_mem *)shmat(shmid, shmadd, 0)) ==         //assigning total
        (shared_mem *)-1){
            perror ("shmat");
            exit(0);
    }

   total->value = 0;

   int increments[] = {100000, 200000, 300000, 500000};
    for (int i = 0; i < 4; ++i){       //parent function forking into four processes
        pid_t pid = fork();
        if (pid == -1){
            perror("fork");
        }
        if (pid == 0){
            process(i, increments[i]);
        }

    }
        


        for (int i = 0; i < 4; ++i){            //parent waiting for the children to exit, then printing their pid
            pid_t child_pid = wait(&status);
            printf("Child with ID: %d has just exited. \n", child_pid);
        }

        if (shmdt(total) == -1){
            perror("shmdt");
            exit(-1);
        }

        if (shmctl(shmid, IPC_RMID, NULL) == -1)
        {
            perror("shmctl");
        }

        shmdt(total);
        shmctl(shmid, IPC_RMID, NULL);

    
    printf("End of program. \n");
    
 
    

    return 0;
}
