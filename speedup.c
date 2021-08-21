#include "crivo.h"

void showPrimes(Data *data) {
  for(int i = 2; i < MAX; i++) {
    if(data->isPrime[i]) {
      printf("%d\n", i);
    }
  }
}

void main() {
  for(int i = 0; i < 1; i++) {
    Data serial, parallel;
    serial.length = 0;
    serial.numThreads = 1;
    parallel.length = 0;
    parallel.numThreads = 1;
    sieveSerial(&serial);
    sieveThreads(&parallel, 2);

    // showPrimes(&serial);
    // showPrimes(&parallel);
    double speedup = serial.time/parallel.time;
    double efficiency = serial.time/(parallel.numThreads * parallel.time);

    printf("==================================================================\n");
    printf("Serial Length: %d\nParallel Length: %d\n", serial.length, parallel.length);

    printf("Serial Time: %lf\nParallel Time: %lf\n", serial.time, parallel.time);
    printf("Thread's number:\n  Serial-%d\n  Parallel-%d\n", serial.numThreads, parallel.numThreads);
    printf("SpeedUp: %lf\n", speedup);
    printf("Efficiency: %lf\n", efficiency);
    printf("==================================================================\n");
    free(serial.isPrime);
    free(parallel.isPrime);
  }
}
