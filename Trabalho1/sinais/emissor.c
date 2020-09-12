/*
* References:
* https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_74/apis/sigkill.htm
* https://www.geeksforgeeks.org/signals-c-language/
* https://en.wikipedia.org/wiki/C_signal_handling
* https://linuxhint.com/signal_handlers_c_programming_language/
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void help();

int main(int argc, char **argv){
    
    int pid, signal, signal_return;

    // verification of parameters
    if (argc != 3){
        help();
        exit(1);
    }
    
    pid = atoi(argv[1]);
    signal = atoi(argv[2]);

    // checking if the process exists
    if(kill(pid, 0) == -1) {
        help();
        exit(1);
    }

    // sending the wanted signal
    signal_return = kill(pid, signal);
    
    return signal_return;
}

// help Function, called when an error occurs
void help(){
    printf(" Este programa recebe como parâmetros o número do processo destino e o sinal que deve ser enviado.\n\n 2) SIGINT \n10) SIGUSR1 \n12) SIGUSR2 \n\n Digite 'kill -l' para mais\n");
}