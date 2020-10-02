#include <iostream> 
#include <atomic> 
#include <vector>
#include <thread> 
#include <list>
#include <math.h>
#include <sys/time.h> 
#include <iostream>     // std::cout, std::fixed
#include <iomanip>      // std::setprecision
using namespace std;

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


void sum(int index, int numbers,vector<int8_t> *list_num, int *result){

  int mini_result = 0;
  for (int i = index; i < index + numbers; i++) { 
    mini_result += list_num->at(i);
  }

  lock.lock();
  *result = *result + mini_result;
  lock.unlock();

} 

void test_sum(int n, int k, vector<int8_t> list_num){

  vector<thread> threads;

  int result = 0;
  int resto = n%k;
  int numbers = n/k;

  struct timeval start, end; 

  for (int i=0; i < k; i ++){
    threads.push_back(thread(sum,i*numbers,numbers, &list_num, &result));
  }

  if(resto != 0){
    threads.push_back(thread(sum, numbers*k,resto, &list_num, &result));
  }

  gettimeofday(&start, NULL);

  for (int i = 0; i < threads.size(); i++){
    threads[i].join();
  } 

  gettimeofday(&end, NULL);

  double time_taken; 

  time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
  time_taken = (time_taken + (end.tv_usec -  
                              start.tv_usec)) * 1e-6; 

  cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(6); 
  cout << " sec" << endl; 

}

int main(){

  srand(time(0));

  vector<double> Ns = {pow(10,7), pow(10,8), pow(10,9)};

  vector<int> Ks = {1, 2, 4, 8, 16, 32, 64, 128, 256};

  for(int i = 0; i < Ns.size(); i++){
    int current_n = (int)Ns[i];
    cout << current_n << " números\n";

    for(int j = 0; j < Ks.size(); j++){
      cout << Ks[i] << " Ks\n";

      int current_k = Ks[i];
      for(int k = 0; k < 10; k++){
        cout << "execução " << k << " \n";

        vector<int8_t> current_vector(current_n, 0);
        for(int l = 0; l < current_n; l++){
          current_vector[i] = (100 - rand()*200);
        }

        test_sum(current_n, current_k, current_vector);

      }

    }
  }

  return 0;
}