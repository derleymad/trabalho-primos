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

void sieve1(Data *data) {
  double start = omp_get_wtime();
  int *a;
  int i;
  a = (int*)malloc(MAX*sizeof(int));

  int numThreads;
  #pragma omp parallel shared(a, i) num_threads(1)
  {
    data->numThreads = omp_get_num_threads();
    int root = sqrt(MAX);
    #pragma omp for
      for(i = 0; i < MAX; i++) {
        a[i] = 1;
      }

    #pragma omp parallel shared(a, i) num_threads(1)
      for(i = 2; i < root; i++) {
        if(a[i]) {
          a[i] = 1;
          int square = i * i;
          #pragma omp for
            for(int j = 0; j < MAX; j++) {
              int index = square + (i * j);
              if(index < MAX) a[index] = 0;
              else j = MAX;
            }
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
  int i;
  a = (int*)malloc(MAX*sizeof(int));

  int numThreads;
  #pragma omp parallel shared(a, i)
  {
    data->numThreads = omp_get_num_threads();
    int root = sqrt(MAX);
    #pragma omp for
      for(i = 0; i < MAX; i++) {
        a[i] = 1;
      }

    #pragma omp parallel shared(a, i)
      for(i = 2; i < root; i++) {
        if(a[i]) {
          a[i] = 1;
          int square = i * i;
          #pragma omp for
            for(int j = 0; j < MAX; j++) {
              int index = square + (i * j);
              if(index < MAX) a[index] = 0;
              else j = MAX;
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
  sieve1(&serial);
  sieveThreads(&parallel);
  double speedup = serial.time/parallel.time;
  double efficiency = serial.time/(parallel.numThreads * parallel.time);

  printf("Serial Time: %lf\nParallel Time: %lf\n", serial.time, parallel.time);
  printf("Thread's number:\n  Serial-%d\n  Parallel-%d\n", serial.numThreads, parallel.numThreads);
  printf("SpeedUp: %lf\n", speedup);
  printf("Efficiency: %lf\n", efficiency);

  free(serial.primes);
  free(parallel.primes);
}