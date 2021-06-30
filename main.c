#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

struct data
{
  int *primes;
  int length;
  int numThreads;
  double time;
};

typedef struct data Data;

#define MAX 10000000

static int a[ ] = { [0 ... 10000000] = 1};

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
  a = (int*)malloc(MAX*sizeof(int));

  for(int i = 0; i < MAX; i++) {
    a[i] = 1;
  }

  for(int i = 2; i < root; i++) {
    if(a[i]) {
      a[i] = 1;
      for(int j = 0; j < MAX; j++) {
        int square = i * i;
        int index = square + (i * j);
        if(index < MAX) a[index] = 0;
        else break;
      }
    }
  }
  setData(data, a);
  free(a);
  double end = omp_get_wtime();
  data->time = end-start;
}

void sieveThreads(Data *data) {
  double start = omp_get_wtime();
  int *a;
  a = (int*)malloc(MAX*sizeof(int));

  int numThreads;
  #pragma omp parallel shared(a)
  {
    data->numThreads = omp_get_num_threads();
    int root = sqrt(MAX);
    #pragma omp for
      for(int i = 0; i < MAX; i++) {
        a[i] = 1;
      }

    #pragma omp for
      for(int i = 2; i < root; i++) {
        if(a[i]) {
          a[i] = 1;
          int square = i * i;
          for(int j = 0; j < MAX; j++) {
            int index = square + (i * j);
            if(index < MAX) a[index] = 0;
            else break;
          }
        }
      }

    /*for(int i = 2; i < n; i++) {
      if(a[i]){
        printf("%d - %d\n", omp_get_thread_num(), i);
      }
    }*/
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
  sieveThreads(&parallel);
  printf("time serial: %lf\ntime parallel: %lf", serial.time, parallel.time);
  printf("\nThread's number:\n\tSerial-%d\n\tParallel-%d\n", serial.numThreads, parallel.numThreads);
  // double speedup = timeSerial/timeThreads;
  // double efficiency = timeSerial/(4*timeThreads);

  // printf("Time Serial: %f\n", timeSerial);
  // printf("Time Parallel: %f\n", timeThreads);
  // printf("Speedup: %f\n", speedup);
  // printf("Efficiency with 4 theads: %f\n", efficiency);
}