#include <iostream> 
#include <atomic> 
#include <vector>
#include <thread> 
#include <list>
#include <math.h>
#include <sys/time.h> 
#include <iostream>
#include <iomanip>   
using namespace std;

struct args {
    int index;
    int numbers;
};

class SpinLock {
    atomic_flag locked = ATOMIC_FLAG_INIT ;
public:
    void lock() {
        while (locked.test_and_set(memory_order_acquire)) { ; }
    }
    void unlock() {
        locked.clear(memory_order_release);
    }
};

SpinLock lock;
vector<int8_t> list_num;
int result;
cpu_set_t cpus;


void *sum(void *input){

  int index = ((struct args*)input)->index;
  int numbers = ((struct args*)input)->numbers;

  int mini_result = 0;
  for (int i = index; i < index + numbers; i++) { 
    mini_result += list_num[i];
  }

  lock.lock();
  result = result + mini_result;
  lock.unlock();

} 

void test_sum(int n, int k){

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  result = 0;
  int resto = n%k;
  int numbers = n/k;

  int num_threads = k;

  if (resto > 0){
    num_threads++;
  }
  
  pthread_t threads[num_threads];
  struct timeval start, end; 

  gettimeofday(&start, NULL);

  for (int i=0; i < k; i ++){
    args *argsToFunc = (struct args *)malloc(sizeof(struct args));
    argsToFunc->index = i*numbers;
    argsToFunc->numbers = numbers;
    pthread_create(&threads[i], &attr, sum, (void *) argsToFunc);
  }

  if(resto != 0){
    args *argsToFunc = (struct args *)malloc(sizeof(struct args));
    argsToFunc->index = k*numbers;
    argsToFunc->numbers = resto;
    pthread_create(&threads[k], &attr, sum, (void *) argsToFunc);
  }

  for( int i=0; i < num_threads; i++){
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&end, NULL);

  double time_taken; 

  time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
  time_taken = (time_taken + (end.tv_usec -  
                              start.tv_usec)) * 1e-6; 

  cout << fixed << time_taken << setprecision(6) << endl; 

}

int main(){

  srand(time(0));

  vector<double> Ns = {pow(10,7), pow(10,8), pow(10,9)};

  vector<int> Ks = {1, 2, 4, 8, 16, 32, 64, 128, 256};

  for(int i = 0; i < Ns.size(); i++){
    int current_n = (int)Ns[i];

    for(int j = 0; j < Ks.size(); j++){
      int current_k = Ks[j];
      
      for(int k = 0; k < 10; k++){
        list_num = vector<int8_t>(current_n, 0);
        
        for(int l = 0; l < current_n; l++){
          list_num[l] = (100 - rand()*200);
        }

        cout << current_n << ";" << current_k << ";" << k << ";";
        test_sum(current_n, current_k);

      }

    }
  }

  return 0;
}