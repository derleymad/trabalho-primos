#include "crivo.h"

void sieveSerial(Data *data)
{
  double start = omp_get_wtime();
  int root = sqrt(MAX);
  int *a, length = 0;
  a = (int *)calloc(MAX, sizeof(int));

  for (int i = 2; i < MAX; i++)
  {
    a[i] = 1;
  }

  for (int i = 2; i < root; i++)
  {
    if (a[i])
    {
      for (int j = 2 * i; j <= MAX; j += i)
      {
        a[j] = 0;
      }
    }
  }

  for (int i = 1; i < MAX; i++)
  {
    length += a[i];
  }

  data->length = length;
  data->isPrime = a;
  double end = omp_get_wtime();
  data->time = end - start;
}

void sieveThreads(Data *data, int numThreads)
{
  double start = omp_get_wtime();
  int *a, length = 0;
  a = (int *)calloc(MAX, sizeof(int));
  int root = sqrt(MAX);
  data->numThreads = numThreads;

#pragma omp parallel for num_threads(numThreads)
  for (int k = 2; k < MAX; k++)
  {
    a[k] = 1;
  }

#pragma omp parallel num_threads(numThreads)
  {
#pragma omp for schedule(dynamic)
    for (int i = 2; i <= root; i++)
    {
      if (a[i])
      {
        for (int j = 2 * i; j <= MAX; j += i)
        {
          a[j] = 0;
        }
      }
    }
  }

#pragma omp parallel for num_threads(numThreads) reduction(+: length)
  for (int i = 0; i < MAX; i++)
  {
    length += a[i];
  }

  data->length = length;
  data->isPrime = a;
  double end = omp_get_wtime();
  data->time = end - start;
}

void sieveMPI(Data *data)
{
  double start = omp_get_wtime();
  MPI_Init(NULL, NULL);
  int nprocs;
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *a = NULL;
  int prime = 2;
  int i, j;
  int *subVet = NULL;
  int root = sqrt(MAX);

  if (rank == 0)
  {
    a = (int *)malloc(MAX * sizeof(int));
  }

  subVet = (int *)malloc(SUBPROC * sizeof(int));
  MPI_Scatter(a, 1, MPI_INT, subVet, 1, MPI_INT, 0, MPI_COMM_WORLD);

  for (i = 0; i < SUBPROC; i++)
  {
    if (rank == 0 && i < 2)
      continue;
    subVet[i] = 1;
  }

  for (i = 2; i < root; i++)
  {
    if (rank == 0 && subVet[i])
    {
      prime = i;
    }
    MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int index = (SUBPROC * rank);
    int begin = (index % prime == 0)? index: (prime - (index % prime)) + index;
    begin -= index;
    // if(rank!= 0) printf("begin: %d, prime: %d\n", begin, prime);
    for (j = (rank == 0)? 2 * prime: begin; j <= SUBPROC; j += prime)
    {
        subVet[j] = 0;
    }
  }

  int subLength = 0;
  int length;
  for(int i = 0; i < SUBPROC; i++) {
    if(subVet[i]){
      subLength++;
    }
  }

  MPI_Gather(subVet, SUBPROC, MPI_INT, a, SUBPROC, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Reduce(&subLength, &length, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  MPI_Finalize();

  data->length = length;
  data->isPrime = a;
  data->numThreads = nprocs;
  double end = omp_get_wtime();
  data->time = end - start;
  return;
}
