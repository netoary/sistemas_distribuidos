#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void help(){
    printf(" Este programa recebe como parâmetros o número do processo destino e o sinal que deve ser enviado.\n\n 2) SIGINT \n10) SIGUSR1 \n12) SIGUSR2 \n\n Digite 'kill -l' para mais\n");
}

int main(int argc, char **argv){
    
    int pid, signal, signal_return;

    if (argc != 3){
        help();
        exit(1);
    }
    
    pid = atoi(argv[1]);
    signal = atoi(argv[2]);

    if(kill(pid, 0) == -1) {
        help();
        exit(1);
    }

    signal_return = kill(pid, signal);
    
    return signal_return;
}

