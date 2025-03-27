#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Second arg - PID, third - address\n");
        return EXIT_FAILURE;
    }

    pid_t passed_pid = atoi(argv[1]);
    //Шіснадцятковий формат адреси
    unsigned long addr = strtoul(argv[2], NULL, 16);

    if (ptrace(PT_ATTACH, passed_pid, NULL, 0) == -1) {
        printf("Couldn't attach to PID");
        return EXIT_FAILURE;
    }

    waitpid(passed_pid, NULL, 0);
    printf("Attached to PID: %d\n", passed_pid);
    //Отримання даних з адреси
    long data = ptrace(PT_READ_D, passed_pid, (void *)addr, 0);
    if(!data){
        printf("Couldn't read from address\n");
    }
    else{
        printf("Address: 0x%lx, data: 0x%lx\n", addr, data);
    }

    if (ptrace(PT_DETACH, passed_pid, NULL, 0) == -1){
        printf("Couldn't detach from PID");
        return EXIT_FAILURE;
    }

    printf("Detached from PID: %d\n", passed_pid);
    return EXIT_SUCCESS;
}
