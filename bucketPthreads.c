/*
   Arquivo: bucketPthreads.c
   
   Objetivo: Implementação do algoritmo de ordenacao Bucket Sort 
   usando Pthreads.
   Funcionamento do algoritmo:
   Divide um vetor em um numero finito de recipientes.
   Cada recipiente é então ordenado individualmente, seja 
   usando um algoritmo de ordenação diferente, ou usando bucket
   sort recursivamente.
   Compilação: gcc bucketPthreads.c -lm -o bucketPthreads -lpthread
   Execução: ./bucketPthreads TamanhodoVetor NumerodeThreads
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

typedef struct bucket{
   int *vet;
   int tam;
   int min;
   int max;
}Balde;


typedef struct bs_args{
     int tam;
     int *v;
}bs_Args;


/* Função para a inserção de cada elemento no balde*/
void insereNoBalde(Balde *balde, int elem){
    balde->vet[balde->tam++] = elem;
}

/* Função auxiliar para a ordenação dos baldes */
void insertionSort(int *vet, int tam){
  int i, j, aux;
  for (i = 0; i < tam; ++i){
     aux = vet[i];
     j = i - 1;
   while(j >= 0 && vet[j] > aux){
     vet[j+1] = vet[j];
     j = j - 1; 
   }
    vet[j+1] = aux;
  }
}


/* Função para a criação dos vetores com valores pseudo-aleatorios*/
int* geraVetor(int tam, int* min, int* max) {
    int *vet = (int *)malloc(sizeof(int) * tam);
    int i;

    srand(time(NULL));
    vet[0] = rand() % RAND_MAX;
    *min = vet[0];
    *max = vet[0];

    for (i = 1; i < tam; ++i) {
        vet[i] = rand() % RAND_MAX;
        if (*max < vet[i]){
          *max = vet[i];
        }
        if (*min > vet[i]){
          *min = vet[i];
        }
    }
    return vet;
}


/* Função utilizada para exibição do vetor */
void exibevetor(int *vet, int tam){
   int i;
   for(i = 0; i < tam; ++i){
      printf("%d ", vet[i]);
   }
   printf("\n");
}

bs_Args *criaArgs(int *vet,int tam){
   bs_Args *args = malloc(sizeof(bs_Args));
   args->v = vet;
   args->tam = tam;
   return args;
}

void *worker(void *args){
   bs_Args *arg = args;
   insertionSort(((bs_Args *)args)->v,((bs_Args*)args)->tam);
   free(arg);
}


int main(int argc, char const *argv[])
{
  int i,j,min,max,counter=0;
  clock_t t, end;
  double cpu_time_used;
  if (argc != 3) {
     fprintf(stderr, "Digite: [tamanho do vetor] [numero de threads]\n");
     exit(1);
  }
   
  int tam = atoi(argv[1]);
  int numThreads = atoi(argv[2]);
  int *vet1 = geraVetor(tam,&min,&max);
  //exibevetor(vet1,tam);

  Balde balde[numThreads];
  pthread_t tid[numThreads];


  int range = (int) ceil((float)(max - min) / (float)(numThreads-1));
	balde[0].min = min;
	balde[0].max = min + range;
	balde[0].vet = (int*) malloc(sizeof(int) * tam);
	balde[0].tam = 0;

  for(i = 1; i < numThreads - 1; ++i) {
      balde[i].min = balde[i - 1].max + 1;
      balde[i].max = balde[i].min + range;
      balde[i].vet = (int*) malloc(sizeof(int) * tam);
      balde[i].tam = 0;
  }

  for(i = 0; i < tam; ++i) {
   for (j = 0; j < numThreads - 1; ++j) {
    if(vet1[i] <= balde[j].max && vet1[i] >= balde[j].min)
        insereNoBalde(&balde[j], vet1[i]);
	    
     }
   }

   for (i = 0; i < numThreads; ++i) {
     pthread_create(&tid[counter++],NULL,worker,criaArgs(balde[i].vet,balde[i].tam));
     if (counter==numThreads){
         counter=0;
        int j;
     for (j = 0; j < numThreads; ++j){
          pthread_join(tid[j],NULL);
      }
   }
  }
   int *r = (int*) malloc(sizeof(int) * tam);
   int cont = 0;
   for (i = 0; i < numThreads; ++i) {
     for (j = 0; j < balde[i].tam; ++j) {
	     r[cont++] = balde[i].vet[j];
    }
   }
	 
   exibevetor(r,tam); 

   t = clock(); 
   
   cpu_time_used = ((double)t)/CLOCKS_PER_SEC;
   printf("\nTempo de execução: %f\n", cpu_time_used);

   free(r);
  
   return 0;
}

