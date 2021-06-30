#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

static int a[ ] = { [0 ... 10000000] = 1};

double sieveSerial() {
  double start = omp_get_wtime();
  int n = 10000000;
  int root = sqrt(n);

  for(int i = 2; i < root; i++) {
    if(a[i]) {
      a[i] = 1;
      for(int j = 0; j < n; j++) {
        int square = i * i;
        int index = square + (i * j);
        if(index < n) a[index] = 0;
        else break;
      }
    }
  }

  /*for(int i = 2; i < n; i++) {
    if(a[i]){
      printf("%d\n", i);
    }
  }*/
  double end = omp_get_wtime();

  return end-start;
}

double sieveThreads() {
  double start = omp_get_wtime();

  #pragma omp parallel num_threads(4)
  {
    int n = 10000000;
    int root = sqrt(n);

    #pragma omp for
      for(int i = 2; i < root; i++) {
        if(a[i]) {
          a[i] = 1;
          for(int j = 0; j < n; j++) {
            int square = i * i;
            int index = square + (i * j);
            if(index < n) a[index] = 0;
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

  double end = omp_get_wtime();

  return end-start;
}

void main() {
  double timeSerial = sieveSerial();
  double timeThreads = sieveThreads();

  double speedup = timeSerial/timeThreads;
  double efficiency = timeSerial/(4*timeThreads);

  printf("Time Serial: %f\n", timeSerial);
  printf("Time Parallel: %f\n", timeThreads);
  printf("Speedup: %f\n", timeSerial);
  printf("Efficiency with 4 theads: %f\n", timeThreads);
}