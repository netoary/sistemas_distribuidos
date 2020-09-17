// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 

int is_prime(int numero){
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

int main(int argc, char const *argv[]) 
{   
    //start of block of code from https://www.geeksforgeeks.org/socket-programming-cc/
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address);  
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    //end of block of code from https://www.geeksforgeeks.org/socket-programming-cc/

    char message[20];
    char response[1];
    int received_num;

    // read from socket and test if is "0"
    while(read(new_socket , message, 20) != -1 && atoi(message) != 0 ) {
        
        printf("Received number: %s\n", message);
        
        // store received_num with int from message
        received_num = strtol(message, NULL, 10);

        // store in response if received_num is prime
        sprintf(response, "%d", is_prime(received_num));

        if(atoi(response) == 1){
            printf("Sending response: is prime\n\n");
        } else {
            printf("Sending response: isn't prime\n\n");
        }
        
        // send response back
        send(new_socket, response, 2, 0);
    }


    printf("Finished\n");     
    return 0; 
} 