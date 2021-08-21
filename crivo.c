#include "crivo.h"

void sieveSerial(Data *data) {
  double start = omp_get_wtime();
  int root = sqrt(MAX);
  int *a, length = 0;
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

  for(int i = 1; i < MAX; i++) {
    length += a[i];
  }

  data->length = length;
  data->isPrime = a;
  double end = omp_get_wtime();
  data->time = end-start;
}

void sieveThreads(Data *data, int numThreads) {
  double start = omp_get_wtime();
  int *a, length = 0;
  a = (int*)calloc(MAX, sizeof(int));
  int root = sqrt(MAX);
  data->numThreads = numThreads;

  #pragma omp parallel for num_threads(numThreads)
    for(int k = 2; k < MAX; k++) {
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

  #pragma omp parallel for num_threads(numThreads) reduction(+: length)
    for(int i = 0; i < MAX; i++) {
      length += a[i];
    }
  
  data->length = length;
  data->isPrime = a;
  double end = omp_get_wtime();
  data->time = end-start;
}

// Falta só fazer o algoritmo MPI
void sieveMPI(Data *data, int numProcess) {
  double start = omp_get_wtime();
  int *a, length = 0;
  a = (int*)calloc(MAX, sizeof(int));
  int root = sqrt(MAX);
  data->numThreads = numProcess;

  #pragma omp parallel for num_threads(numProcess)
    for(int k = 2; k < MAX; k++) {
        a[k] = 1;
    }

  #pragma omp parallel num_threads(numProcess)
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

  #pragma omp parallel for num_threads(numProcess) reduction(+: length)
    for(int i = 0; i < MAX; i++) {
      length += a[i];
    }
  
  data->length = length;
  data->isPrime = a;
  double end = omp_get_wtime();
  data->time = end-start;
}

