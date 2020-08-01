/* Contador de primos
 *
 * Este programa recebera uma serie de numeros em sua entrada. 
 * Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de primos que recebeu (2,3,5,7,11... são numeros primos)
 * Cada numero deve ser analisado em um processor diiferente
 * usando 4 processor maximo
 *Os numeros tem tamanho macimo de 2^64 <=> long long int 
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

#define N_PROCESS_MAX 4

//saber se eh primo
bool eh_primo(int x){
	bool eh= true;
	// 0 e 1 não são primos
	if(x < 2) eh =false;
	
	for (int i = 2; i < x; i++){
		if (x % i == 0){
      eh=false;
		}
	}
	return eh;
}

// main program
int main() {

	// entrada
	long long int entrada[100];  
	char c; 

	// Definir flags de protecao e visibilidade de memoria
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;

	// Criar area de memoria compartilhada
	int *cuantos_primos = (int *) mmap(NULL, sizeof(int), protection, visibility, 0, 0);

	// int na entrada
	int i = 0;
	while(c != '\n'){
		scanf("%llu", &entrada[i]);
		c = getchar();
		i++;
	}

	int entrada_size = i;

	// Eligir o numero de processo que vamos usar
	int n_processor = N_PROCESS_MAX;
	if(entrada_size < N_PROCESS_MAX) n_processor = entrada_size;
	
	pid_t filhos[n_processor];
	// creates pipe
	int pipefd[2]; // pipe r=0/w=1
	pipe(pipefd);

	// Escrever os int no pipe
	for(i = 0; i < entrada_size; i++){
		write(pipefd[1], &entrada[i], sizeof(entrada[i]));
	}
	
	//Fechar pipe write
	close(pipefd[1]);

	// crear o numero de processes necessario
	for(i = 0; i < n_processor; i++){
		
		filhos[i] = fork();

		// Esta parte do codigo executa no processo filho
		if(filhos[i] == 0){
			long long int numero;
			
			// Fechar pipe write
			close(pipefd[1]);

			// reads pipe
			while(read(pipefd[0], &numero, sizeof(numero)) > 0){

				if(eh_primo(numero)) (*cuantos_primos)++;
			}

			// fechar pipe read
			close(pipefd[0]);
				
			// Sair do processo
			exit(0);
		}
	}

	// Todos os filhos foram gerados. Esperando...
	for(int k = 0; k < n_processor; k++){
		waitpid(filhos[k], NULL, 0);
	}
	// Escrever o numero de primos
	printf("%d\n", (*cuantos_primos));
	return 0;
}
