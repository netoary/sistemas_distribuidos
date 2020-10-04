#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

/*
// CPU time
int main() 
{ 
    clock_t start, end;
    double cpu_time_used;

    double times[10];

    int i;


    FILE * fp;

    // fp = fopen ("./resul1.txt","w");
    // fp = fopen ("./resul2.txt","w");
    // fp = fopen ("./resul4.txt","w");
    // fp = fopen ("./resul16.txt","w");
    fp = fopen ("./resul32.txt","w");

    double tempo_total = 0;
    double tempo_total2 = 0;
    clock_t t;
    char buffer[100];
    for (int n = 1; n < 17; n*=2) {
        for (i = 0; i < 10; i++) {
            t = clock();
            //system("./item2 1 1 %d", n);
            snprintf(buffer, 100, "./semaforo 1 %d\n", n); 
            system(buffer);
            t = clock() - t; 

            double tempo_segundos = ((double) t)/CLOCKS_PER_SEC;
            tempo_segundos = tempo_segundos*10;
            fprintf (fp, "1;%d;%f \n", n, tempo_segundos);
            tempo_total += tempo_segundos;
        }
        // fprintf (fp, "1, %d em %f \n", n, tempo_total/10);
        // printf("1, %d em %f \n", n, tempo_total/10);
        for (i = 0; i < 10; i++) {
            t = clock();
            //system("./item2 1 1 %d", n);
            snprintf(buffer, 100, "./semaforo %d 1\n", n); 
            system(buffer);
            t = clock() - t; 

            double tempo_segundos = ((double) t)/CLOCKS_PER_SEC;
            tempo_segundos = tempo_segundos*10;
            fprintf (fp, "%d;1;%f \n", n, tempo_segundos);
            tempo_total2 += tempo_segundos;
        }
        // fprintf (fp, "%d, 1 em %f \n", n, tempo_total2/10);
    }

    fclose (fp);
}
*/

// real time
int main() 
{ 
    clock_t start, end;
    double cpu_time_used;

    double times[10];

    int i;

    FILE * fp;

    // fp = fopen ("./resul1real.txt","w");
    // fp = fopen ("./resul2real.txt","w");
    // fp = fopen ("./resul4real.txt","w");
    // fp = fopen ("./resul16real.txt","w");
    fp = fopen ("./resul32real.txt","w");


    struct timeval begin, fim;
 
    char buffer[100];
    for (int n = 1; n < 17; n*=2) {
        for (i = 0; i < 10; i++) {
            gettimeofday(&begin, 0);

            snprintf(buffer, 100, "./semaforo 1 %d\n", n); 
            system(buffer);

            gettimeofday(&fim, 0);
            long seconds = fim.tv_sec - begin.tv_sec;
            long microseconds = fim.tv_usec - begin.tv_usec;
            double elapsed = seconds + microseconds*1e-6;
            fprintf (fp, "1;%d;%f \n", n, elapsed);
        }
        for (i = 0; i < 10; i++) {
            gettimeofday(&begin, 0);

            snprintf(buffer, 100, "./semaforo %d 1\n", n); 
            system(buffer);

            gettimeofday(&fim, 0);
            long seconds = fim.tv_sec - begin.tv_sec;
            long microseconds = fim.tv_usec - begin.tv_usec;
            double elapsed = seconds + microseconds*1e-6;
            fprintf (fp, "%d;1;%f \n", n, elapsed);
        }
    }

    fclose (fp);
    
}
