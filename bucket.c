
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
struct Compara{
    int valor;
    int indice;
};
void bucketSort(int *vet,int tam);
struct Compara procuraMaior(int *vet,int tam);
/* Função utilizada para exibição do vetor */
void exibevetor(int *vet, int tam){
   int i;
   for(i = 0; i < tam; ++i){
      printf("%d\n", vet[i]);
   }
   printf("\n");
}


/* Função para a criação dos vetores com valores pseudo-aleatorios*/
void geraVetor(int *vet,int tam){
    int i;
    for(i = 0; i < tam; ++i){
    	//srand(time(0));
       vet[i] = rand() % RAND_MAX;
    }
}

/* implementação do bucketSort com OpenMP*/
void bucketSort(int *vet,int tam){
	int i,aux,m;
	struct Compara maior;
    maior = procuraMaior(vet,tam);
	m = maior.valor;
	int vet2[m-1];
	vet2[m] = 30;
	//printf("shdsydhsdh %ld \n",vet2[m]);
	 #pragma omp parallel  for num_threads(8) 
  	{
  		 for (i = 1; i < m-1; ++i)
  		{
  			vet2[i-1] = m;
  		}
   		for(i=0; i<tam;++i){
   			aux = vet[i];
   			vet2[aux]= aux;
   		}
   		aux =0;
		for ( i = 0; i < m; ++i)
		{
			if(vet2[i]!= (m+1)){
				vet[aux] = vet2[i];
				++aux;
			}
		}
  	} 

}
struct Compara procuraMaior(int *vet,int tam){
	int i,j,x;
	#pragma omp declare reduction(max : struct Compara : omp_out = omp_in.valor > omp_out.valor ? omp_in : omp_out)
	 struct Compara maior;
    /* diretiva omp simd aplicada ao laço indica que multiplas iteracoes do loop podem ser executadas concorrentemente usando instrucoes SIMD */
    #pragma omp simd reduction(max:maior)  //vetorizacao com reducao
    	maior.valor = vet[0];
      	maior.indice = 0;  //armazena indice do menor elemento
    for (i = 0; i < (tam - 1); ++i){ 
        for (j = i + 1; j < tam; ++j){
        	x = vet[j];
            if (x > maior.valor){ // busca pelo elemento de menor valor 
               	maior.valor = vet[j];
                maior.indice = j; // salva o novo indice como menor 
            }
        }
        /* troca e coloca o menor elemento para frente */
    }
    return maior;

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