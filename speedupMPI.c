#include "crivo.h"

void showPrimes(Data *data) {
  for(int i = 2; i < MAX; i++) {
    if(data->isPrime[i]) {
      printf("%d\n", i);
    }
  }
}

void main() {
  int numRep = 1;
  double speedup = 0.0, efficiency = 0.0;

  for(int i = 0; i < numRep; i++) {
    Data serial, mpi;
    serial.length = 0;
    serial.numThreads = 1;
    mpi.length = 0;
    sieveSerial(&serial);
    sieveMPI(&mpi);

    speedup += serial.time/mpi.time;
    efficiency += serial.time/(mpi.numThreads * mpi.time);

    printf("==================================================================\n");
    printf("Serial Length: %d\nMPI Length: %d\n", serial.length, mpi.length);

    printf("Serial Time: %lf\nMPI Time: %lf\n", serial.time, mpi.time);
    printf("Thread's number:\n  Serial-%d\n  MPI-%d\n", serial.numThreads, mpi.numThreads);
    printf("SpeedUp: %lf\n", serial.time/mpi.time);
    printf("Efficiency: %lf\n", serial.time/(mpi.numThreads * mpi.time));
    printf("==================================================================\n");
    free(serial.isPrime);
    free(mpi.isPrime);
  }
  /*printf("Speedup average with %d repetitions (MPI): %lf\n", numRep, speedup/numRep);
  printf("Efficiency average with %d repetitions (MPI): %lf\n", numRep, efficiency/numRep);
  printf("==================================================================\n");*/
}
