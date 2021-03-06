#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

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
    // fp = fopen ("./resul32real.txt","w");

    fp = fopen ("./result.csv","w");

    struct timeval begin, fim;
 
    char buffer[100];
    for (int mem = 1; mem < 33; mem*=2) {
        for (int n = 1; n < 17; n*=2) {
            for (i = 0; i < 10; i++) {
                gettimeofday(&begin, 0);

                snprintf(buffer, 100, "./semaforo 1 %d %d\n ", n, mem); 
                system(buffer);

                gettimeofday(&fim, 0);
                long seconds = fim.tv_sec - begin.tv_sec;
                long microseconds = fim.tv_usec - begin.tv_usec;
                double elapsed = seconds + microseconds*1e-6;
                fprintf (fp, "%d;1;%d;%f \n", mem, n, elapsed);
            }
            for (i = 0; i < 10; i++) {
                gettimeofday(&begin, 0);

                snprintf(buffer, 100, "./semaforo %d 1 %d\n", n, mem); 
                system(buffer);

                gettimeofday(&fim, 0);
                long seconds = fim.tv_sec - begin.tv_sec;
                long microseconds = fim.tv_usec - begin.tv_usec;
                double elapsed = seconds + microseconds*1e-6;
                fprintf (fp, "%d;%d;1;%f \n", mem, n, elapsed);
            }
        }
    }

    fclose (fp);
    
}
