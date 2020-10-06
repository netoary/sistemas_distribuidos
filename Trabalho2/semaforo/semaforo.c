/*
* References:
* https://shivammitra.com/c/producer-consumer-problem-in-c/#
* https://medium.com/@sohamshah456/producer-consumer-programming-with-c-d0d47b8f103f


* compile:
* gcc -pthread semaforo.c -o semaforo
*/


#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define M 100000

int* shared_memory;
int consumed;
int bufferSize = 32;

// Semáforos
sem_t mutex;
sem_t empty;
sem_t full;

void *producer();
void *consumer();
int find();
int is_prime(int numero);
void help();
int find_empty();
int find_full();


int main(int argc, char** argv)
{   
    srand(time(0)); 

    if (argc < 3)
    {
        help();
        exit(1);
    }

    // Recupera os parâmetros passados
    int producer_number = atoi(argv[1]);
    int consumer_number =  atoi(argv[2]);
    bufferSize =  atoi(argv[3]);

    if (producer_number == 0 || consumer_number == 0 || bufferSize == 0)
    {
        help();
        exit(1);
    }

    shared_memory = (int*) calloc(bufferSize, sizeof(int));

    pthread_t pro[producer_number],con[consumer_number];
    // pthread_mutex_init(&mutex, NULL);
    sem_init(&mutex, 0, 1);
    sem_init(&empty,0,bufferSize);
    sem_init(&full,0,0);

    consumed = 0;

    int a[5] = {1,2,3,4,5}; //Just used for numbering the producer and consumer

    for(int i = 0; i < producer_number; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i = 0; i < consumer_number; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    while(consumed < M);

    for(int i = 0; i < producer_number; i++) {
        pthread_cancel(pro[i]);
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < consumer_number; i++) {
        pthread_cancel(con[i]);
        pthread_join(con[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    free(shared_memory);

    return 0;
    
}

void *producer(void *pno)
{   
    int item;
    while(1) 
    {
        item = rand()%10000000 + 1; // Produce an random item

        sem_wait(&empty);
        sem_wait(&mutex);

        int free_space = find_empty();
        shared_memory[free_space] = item;

        sem_post(&mutex);    
        sem_post(&full);
    }
}

void *consumer(void *cno)
{   
    while(1) 
    {
        sem_wait(&full);
        sem_wait(&mutex);

        int flag = -1;

        if (consumed < M)
        {
            int full_space = find_full();
            
            flag = shared_memory[full_space];
            shared_memory[full_space] = 0;

            consumed++;
        }

        sem_post(&mutex);
        sem_post(&empty);

        if (flag != -1)
        {
            if(is_prime(flag) == 1)
            {
                printf("%ld é primo!\n", flag);
            }else
            {
                printf("%ld NÃO é primo!\n", flag);
            }
        }
    }
}

int find_empty()
{
    for (int i = 0; i < bufferSize; i++)
    {
        if (shared_memory[i] == 0)
        {
            return i;
        }

    }
}

int find_full()
{
    for (int i = 0; i < bufferSize; i++)
    {
        if (shared_memory[i] != 0)
        {
            return i;
        }

    }
}

int is_prime(int numero){
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

// help Function, called when an error occurs
void help(){
    printf("Este programa recebe como parâmetros o número de threads do tipo produtor e consumidor e o tamanho do vetor N");
}
