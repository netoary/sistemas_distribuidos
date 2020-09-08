#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

int e_primo(int numero){
    //from https://en.wikipedia.org/wiki/Primality_test
    if (numero <= 3){
        return 1;
    }else if(numero % 2 == 0 || numero % 3 == 0){
        return 0;
    }
    int i = 5;
    while (i * i <= numero){
        if(numero % i == 0 || numero % (i+2) == 0){
            return 0;
        }
        i += 6;
    }
    return 1;
}

void help(){
    printf("Este programa recebe como parâmetros o números de números a serem gerados (ex.1000");
}

// https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/
int main(int argc, char **argv){
    int limit, fork_return;
    int fd1[2];

    srand(time(0)); 

    if(argc < 2){
        help();
        return 1; 
    }

    limit = atoi(argv[1]);

    if(pipe(fd1)==-1){ 
        fprintf(stderr, "Erro na criação do Pipe"); 
        return 1; 
    }

    fork_return = fork();

    if(fork_return < 0){ 
        fprintf(stderr, "Erro no Fork"); 
        return 1; 
    }else if(fork_return > 0){ // Processo pai
        int delta = 1;
        char mensage[20];
        
        close(fd1[0]);

        delta = 1;

        for(int i = 0; i < limit; i++){
            delta += (1 + rand()%100);
            printf("Número produzido: %d\n", delta);
            sprintf(mensage, "%d", delta);
            write(fd1[1], mensage, 20);
        }

        sprintf(mensage, "%d", 0);
        write(fd1[1], mensage, 20);

        close(fd1[1]);
        printf("\nfinalizando processo pai!\n\n");
        exit(0);
    }else{ //processo filho
        int numero;
        char mensage[20];

        close(fd1[1]);

        read(fd1[0], mensage, 20);
        numero = atoi(mensage);

        while(numero != 0){
            if(e_primo(numero) == 1){
                printf("%d é primo!\n", numero);
            }else{
                printf("%d NÃO é primo!\n", numero);
            }
            read(fd1[0], mensage, 20);
            numero = atoi(mensage);
        }
        close(fd1[0]);
        printf("\nfinalizando processo filho!\n\n");
        exit(0);
    }

    return 0;
}