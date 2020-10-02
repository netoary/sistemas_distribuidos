// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080

void help();

int main(int argc, char const *argv[]) 
{   
    srand(time(0)); 

    int num_to_generate;

    if(argc < 2){
        help();
        return 1; 
    }

    num_to_generate = atoi(argv[1]);
    
    //Start of block of code from https://www.geeksforgeeks.org/socket-programming-cc/
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    //End of block of code from https://www.geeksforgeeks.org/socket-programming-cc/

    int delta = 1;

    char message[20];
    char prime_variable[1];

    for(int i = 0; i < num_to_generate; i++){
        //generate random number
        delta += (1 + rand()%100);
        printf("Produced number: %d\n", delta);
        
        //write random number into var message
        sprintf(message, "%d", delta);
        
        //send var message in socket
        send(sock,message,20,0);

        printf("Sending number...\n");

        //read response from socket and put in prime variable
        read(sock,prime_variable, 2);
        
        // test if prime variable is 1 or 0
        if(atoi(prime_variable) == 1) {
			printf("Received: is prime\n\n");
		} else {
			printf("Received: isn't prime\n\n");
		}
    }

    // send 0 to socket to socket
    send(sock,"0",20,0);

    printf("Finished\n"); 
    return 0; 
}

void help(){
    printf("Este programa recebe como parâmetros a quantidade de números a serem gerados (ex.1000");
}