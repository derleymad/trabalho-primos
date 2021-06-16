#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

void sieve() {
  #pragma omp parallel num_threads(4)
  {
    int n =  10000000;
    int a[ ] = { [0 ...  10000000] = 1}; 
    int root = sqrt(n);
    #pragma omp for
      for(int i = 2; i < root; i++) {
        if(a[i]) {
          a[i] = 1;
          for(int j = 0; j < n; j++) {
            int square = i * i;
            int index = square + (i * j);
            if(index < n) {
              a[index] = 0;
            } else {
              break;
            }
          }
        }
      }
    for(int i = 2; i < n; i++) {
      if(a[i]){
        printf("%d\n", i);
      }
    }
  }
}

void main() {
  sieve();
}