# Projeto Crivo de Eratóstenes

O Crivo de Eratóstenes é um algoritmo para encontrar números primos até um certo valor limite. O projeto consiste em paralelizar o algoritmo do crivo, para obter um melhor desempenho.

Códigos de compilação para o OpenMP e o MPI, e execução
```
mpicc -fopenmp speedupMPI.c crivo.c -o test
mpicc -fopenmp speedup.c crivo.c -o test
mpiexec -n num_proc ./test
./test
```