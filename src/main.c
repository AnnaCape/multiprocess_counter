/* Contador de primos
 *
 * Este programa recebera uma serie de numeros em sua entrada. 
 * Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de primos que recebeu (2,3,5,7,11... são numeros primos)
 * Cada numero deve ser analisado em um processor diiferente
 * usando 4 processor maximo
 *Os numeros tem tamanho macimo de 2^64 <=> long long int 
 */
#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define N_PROCESS_MAX 4

int main() {
  
  	// entrada
	long long int entrada[100];  
	char c; 
	int i = 0;
	// int na entrada
	while(c != '\n'){
		scanf("%lld", &entrada[i]);
		c = getchar();
		i++;
	}
  int entrada_size = i;
  
  // Definir flags de protecao e visibilidade de memoria
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  // Criar area de memoria compartilhada
  int *cuantos_primos, *p_ativos;
  cuantos_primos = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
  p_ativos = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
  //p_ativos = quantos processos estão funcionando atualmente
  (*p_ativos) = 1;
  (*cuantos_primos)=0;

  //definir o vetor de pid
	pid_t filhos[N_PROCESS_MAX];
	int id=0;
	for(int k=0;k<entrada_size; k++){

   if((*p_ativos)> N_PROCESS_MAX){ 
      for(int w=k-3;w<k;w++){
        waitpid(filhos[w], NULL, 0);
        (*p_ativos)--;
      }
   }
      filhos[k] = fork();
      (*p_ativos)++;


      if(filhos[k] == 0){
          if(entrada[k]!=2 && entrada[k]!=3 && entrada[k]!=5 && entrada[k]!=7){
            if(entrada[k]<=1);
            else if(entrada[k]%2==0);
            else if(entrada[k]%3==0);
            else if(entrada[k]%5==0);
            else if(entrada[k]%7==0);
            else(*cuantos_primos)++;

          }
          else (*cuantos_primos)++;
        exit(0);/*sair do processo*/
      }
	}
  
  // Todos os filhos foram gerados. Esperando...
  for(int j = 0; j<i;j++){
    waitpid(filhos[j], NULL, 0);
  }
  
  // Escrever o numero de primos
  printf("%d\n", *cuantos_primos);

  return 0;
}
