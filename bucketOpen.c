/*
   Arquivo: bucketOpen.c
   
   Objetivo: Implementação do algoritmo de ordenacao Bucket Sort 
   usando OpenMP.

   Funcionamento do algoritmo:
   Divide um vetor em um numero finito de recipientes.
   Cada recipiente é então ordenado individualmente, seja 
   usando um algoritmo de ordenação diferente, ou usando bucket
   sort recursivamente.

   Compilação: gcc -o bucketOpen -fopenmp bucketOpen.c 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define TAM1 1000
#define TAM2 10000
#define TAM3 20000

#define MAX_THREADS 8

/* Função utilizada para exibição do vetor */
void exibevetor(int *vet, int tam){
   int i;
   for(i = 0; i < tam; ++i){
      printf("%d ", vet[i]);
   }
   printf("\n");
}


/* Função para a criação dos vetores com valores pseudo-aleatorios*/
void geraVetor(int *vet,int tam){
    int i;
    for(i = 0; i < tam; ++i){
       vet[i] = rand() % RAND_MAX;
    }
}

/* implementação do bucketSort com OpenMP*/
void bucketSort(int *vet,int tam){

}

int main(int argc, char const *argv[])
{
    int *vet1,*vet2,*vet3;
    int i, tam;
    clock_t t, end;
    double cpu_time_used;

    vet1 = malloc(sizeof(int)*TAM1);
    //vet2 = malloc(sizeof(int)*TAM2);
    //vet3 = malloc(sizeof(int)*TAM3);
    if(vet1==NULL || vet2==NULL || vet3==NULL){
        exit(1);
    }

    geraVetor(vet1,TAM1);
    //geraVetor(vet2,TAM2);
    //geraVetor(vet3,TAM3);
    
    t = clock();
    exibevetor(vet1,TAM1);
    bucketSort(vet1,TAM1);
    t = clock()-t; 
    exibevetor(vet1,TAM1);
    cpu_time_used = ((double)t)/CLOCKS_PER_SEC;
    printf("\nTempo de execução: %f\n", cpu_time_used);
    free(vet1);
    //free(vet2);
    //free(vet3);

    return 0;
}
