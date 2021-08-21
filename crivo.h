#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include <math.h>

#define MAX 10000000

struct data
{
  int *isPrime;
  int length;
  int numThreads;
  double time;
};

typedef struct data Data;

void sieveSerial(Data *data);

void sieveThreads(Data *data, int numThreads);

void sieveMPI(Data *data, int numProcess);