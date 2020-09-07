#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void help(){
    printf("Este programa recebe como parâmetros o número 0 para busy wait e 1 para blocking wait.\n");
}

void handle_SIGUSR1(int signal){
    printf("SIGUSR1 recebido.\n");
}

void handle_SIGUSR2(int signal){
    printf("SIGUSR2 recebido.\n");
}

void handle_SIGINT(int signal){
    printf("SIGINT recebido.\n");
    exit(0);
}


int main(int argc, char **argv){
    
    int type, error;

    if (argc != 2){
        help();
        exit(1);
    }
    printf("ID do processo: %d\n", getpid());

    type = atoi(argv[1]);

    signal(SIGUSR1, handle_SIGUSR1); 
    signal(SIGUSR2, handle_SIGUSR2);
    signal(SIGINT, handle_SIGINT);

    if (type == 0){
        printf("Tipo de espera 'busy wait'.\n");
        while(1);
    }else if(type == 1){
        printf("Tipo de espera 'blocking wait'.\n");
        while(1){
            pause();
        }
    }else{
        help();
        exit(1);
    }

    return 0;
}

