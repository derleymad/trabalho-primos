#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define MAX 10000000

struct data
{
  int *primes;
  int length;
  int numThreads;
  double time;
};

typedef struct data Data;

void setData(Data *data, int *a) {
  data->primes = (int*)malloc(sizeof(int));
  for(int i = 2; i < MAX; i++) {
    if(a[i]) {
      data->primes[data->length] = i;
      data->length++;
      int newSize = data->length + 1;
      data->primes = (int*)realloc(data->primes, newSize * sizeof(Data));
    }
  }
}

void sieveSerial(Data *data) {
  double start = omp_get_wtime();
  int root = sqrt(MAX);
  int *a;
  a = (int*)calloc(MAX, sizeof(int));

  for(int i = 2; i < MAX; i++) {
    a[i] = 1;
  }

  for(int i = 2; i < root; i++) {
    if(a[i]) {
      for(int j = 2 * i; j <= MAX; j += i) {
        a[j] = 0;
      }
    }
  }

  setData(data, a);
  free(a);
  double end = omp_get_wtime();
  data->time = end-start;
}

void sieveThreads(Data *data, int numThreads) {
  double start = omp_get_wtime();
  int *a;
  a = (int*)calloc(MAX, sizeof(int));
  int root = sqrt(MAX);
  data->numThreads = numThreads;

  #pragma omp parallel for num_threads(numThreads)
    for(int k = 0; k < MAX; k++) {
        a[k] = 1;
    }

  #pragma omp parallel num_threads(numThreads)
  {
    #pragma omp for schedule(dynamic)
      for(int i = 2; i <= root; i++) {
        if(a[i]){
          for(int j = 2 * i; j <= MAX; j += i) {
            a[j] = 0;
          }
        }
    }
  } 

  setData(data, a);
  free(a);
  double end = omp_get_wtime();
  data->time = end-start;
}

void main() {
  Data serial, parallel;
  serial.length = 0;
  serial.numThreads = 1;
  parallel.length = 0;
  parallel.numThreads = 1;
  sieveSerial(&serial);
  sieveThreads(&parallel, 2);
  double speedup = serial.time/parallel.time;
  double efficiency = serial.time/(parallel.numThreads * parallel.time);

  printf("Serial Length: %d\nParallel Length: %d\n", serial.length, parallel.length);

  printf("Serial Time: %lf\nParallel Time: %lf\n", serial.time, parallel.time);
  printf("Thread's number:\n  Serial-%d\n  Parallel-%d\n", serial.numThreads, parallel.numThreads);
  printf("SpeedUp: %lf\n", speedup);
  printf("Efficiency: %lf\n", efficiency);

  free(serial.primes);
  free(parallel.primes);
}