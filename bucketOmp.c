/*
   Arquivo: bucketOmp.c
   
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
#include <math.h>
#include <omp.h>

#define TAM1 1000
#define TAM2 10000
#define TAM3 20000

#define MAX_THREADS 8

#define NUMBALDES TAM3/MAX_THREADS

struct Compara{
    int valor;
    int indice;
};

typedef struct node{
    int elem;
    struct node *prox;
}node;

typedef struct balde{
    node *inicio,*fim;
}bucket;


void bucketSort(int *vet,int tam);
struct Compara procuraMaior(int *vet,int tam);

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

/* Função auxiliar para procura do maior valor no vetor*/
/*struct Compara procuraMaior(int *vet,int tam){
  int i,j;    
  #pragma omp declare reduction(max : struct Compara : omp_out = omp_in.valor > omp_out.valor ? omp_in : omp_out)
   struct Compara maior;
    #pragma omp parallel for reduction(max:maior) 
      for(i = 0; i < tam; i++){
        if(vet[i] > maior.valor){
            maior.valor = vet[i];
            maior.indice = i;
        }
    }
    return maior;
}
*/
/* Função auxiliar para a ordenação dos baldes */
void insertionSort(int *vet, int tam){
  int i, j, aux;
  for(i = 0; i < tam; ++i){
     aux = vet[i];
     j = i - 1;
  while(j >= 0 && vet[j] > aux){
     vet[j+1] = vet[j];
     j = j - 1; 
  }
     vet[j+1] = aux;
  }
}

node *criaNodo(int elem){
  node *novo = malloc(sizeof(node));
  if(novo==NULL){
    exit(1);
  }
    novo->elem = elem;
    novo->prox = NULL;
    return novo;
}

void empilha(bucket *balde, int elem){
  node *novo = criaNodo(elem);
  if(balde->inicio==NULL){
     balde->inicio = balde->fim = novo;
  }else{
     balde->fim->prox = novo;
     balde->fim = novo;
   }
}


int desempilha(bucket *balde){
   int tmp;
   node *aux = balde->inicio;
   if(aux==NULL){
    exit(1);
   }

   if(balde->inicio==balde->fim){
     balde->fim = balde->inicio = NULL;
   }else{
     balde->inicio = aux->prox;
     tmp = aux->elem;
   }
   free(aux);
   return tmp;
}

void inicializaFila(bucket *balde){
    balde->inicio = balde->fim = NULL;
}


/* implementação do bucketSort com OpenMP*/
void bucketSort(int *vet,int tam){
  bucket balde[NUMBALDES]; 
	int i,j,k,cont;
	//struct Compara maior;
  //maior = procuraMaior(vet,tam);
	//indice = maior.valor;

 // printf("Maior valor do vetor: %d \n",indice);
	 #pragma omp parallel for num_threads(MAX_THREADS) 
  	 for (i = 0; i < NUMBALDES; ++i){
          inicializaFila(&balde[i]);
      }
     for(i = 0; i < tam; ++i){
       empilha(&balde[(int)vet[i]*10],vet[i]);
     }

     int aux[tam];
     for(i = 0, j = 0; i < NUMBALDES; ++i){
        k = 0;
        while(balde[i].inicio != NULL){
           aux[k++] = desempilha(&balde[i]);
        }
        insertionSort(aux,k);
        for(cont = 0; cont < k && j < tam; ++cont){
            vet[j++] = aux[cont];
        }
     }

  
    
}


int main(int argc, char const *argv[])
{
    int *vet1,*vet2,*vet3;
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
    //exibevetor(vet1,TAM1);
   
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