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
    Data serial, mpi;
    serial.length = 0;
    serial.numThreads = 1;
    mpi.length = 0;
    sieveSerial(&serial);
    sieveMPI(&mpi);

    double speedup = serial.time/mpi.time;
    double efficiency = serial.time/(mpi.numThreads * mpi.time);

    printf("==================================================================\n");
    printf("Serial Length: %d\nMPI Length: %d\n", serial.length, mpi.length);

    printf("Serial Time: %lf\nMPI Time: %lf\n", serial.time, mpi.time);
    printf("Thread's number:\n  Serial-%d\n  MPI-%d\n", serial.numThreads, mpi.numThreads);
    printf("SpeedUp: %lf\n", speedup);
    printf("Efficiency: %lf\n", efficiency);
    printf("==================================================================\n");
    free(serial.isPrime);
    free(mpi.isPrime);
  }
}
