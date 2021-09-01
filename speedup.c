#include "crivo.h"

void showPrimes(Data *data) {
  for(int i = 2; i < MAX; i++) {
    if(data->isPrime[i]) {
      printf("%d\n", i);
    }
  }
}

void main() {
  int numRep = 10, numThreads = 6;
  double speedup = 0.0, efficiency = 0.0;

  for(int i = 0; i < numRep; i++) {
    Data serial, parallel;
    serial.length = 0;
    serial.numThreads = 1;
    parallel.length = 0;
    parallel.numThreads = 1;
    sieveSerial(&serial);
    sieveThreads(&parallel, numThreads);

    speedup += serial.time/parallel.time;
    efficiency += serial.time/(parallel.numThreads * parallel.time);

    printf("==================================================================\n");
    printf("Serial Length: %d\nParallel Length: %d\n", serial.length, parallel.length);
    printf("Serial Time: %lf\nParallel Time: %lf\n", serial.time, parallel.time);
    printf("Thread's number:\n  Serial-%d\n  Parallel-%d\n", serial.numThreads, parallel.numThreads);
    printf("Speedup: %lf\n", serial.time/parallel.time);
    printf("Efficiency: %lf\n", serial.time/(parallel.numThreads * parallel.time));
    printf("==================================================================\n");
    free(serial.isPrime);
    free(parallel.isPrime);
  }
  printf("Speedup average with %d repetitions and %d threads: %lf\n", numRep, numThreads, speedup/numRep);
  printf("Efficiency average with %d repetitions and %d threads: %lf\n", numRep, numThreads, efficiency/numRep);
  printf("==================================================================\n");
}
