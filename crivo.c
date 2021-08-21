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
void sieveMPI(Data *data) {
  double start = omp_get_wtime();
  MPI_Init(NULL,NULL);
  int nprocs;
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *a = NULL;
  int i, j, test, subLength;
  int *tamanhos = NULL;
  int *subVet = NULL;
  int root = sqrt(MAX);

  if(rank == 0){
    a = (int*)malloc(MAX * sizeof(int));
    tamanhos = malloc((nprocs) * sizeof(int));
  }

  subLength = 0;
  subVet = (int*)malloc(SUBPROC * sizeof(int));
  MPI_Scatter(a, SUBPROC, MPI_INT, subVet, SUBPROC, MPI_INT, 0, MPI_COMM_WORLD);

  for(i = 2; i < SUBPROC; i++){
    subVet[i] = 1;
  }

  // test = (MAX / (rank + 1));
  // for(i = 2; i < root;) {
  //   if(rank == 0) i++;
  //   if(subVet[test % i]){
  //     if(rank == 0) {
  //       MPI_Bcast(&i, 1, MPI_INT, 0, MPI_COMM_WORLD);
  //     } else {
  //       MPI_Bcast(&i, 1, MPI_INT, 0, MPI_COMM_WORLD);
  //     }
  //     for(j = 2 * (test % i); j <= SUBPROC; j += i) {
  //       subVet[j] = 0;
  //     }
  //   }
  // }

  for(i = 0; i < SUBPROC; i++){
    subLength += subVet[i];
  }
  
  MPI_Gather(&subLength, 1, MPI_INT, tamanhos, 1, MPI_INT, 0, MPI_COMM_WORLD);
  // MPI_Gather(&subVet, SUBPROC, MPI_INT, a, MAX, MPI_INT, 0, MPI_COMM_WORLD);

  if(rank == 0) {
    for(i = 0; i < nprocs; i++){
      data->length += tamanhos[i];
    }
    data->numThreads = nprocs;
    data->isPrime = a;
  }

  MPI_Finalize();
  double end = omp_get_wtime();
  data->time = end-start;
}

