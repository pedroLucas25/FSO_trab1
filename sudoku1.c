#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


int ler_termo(FILE *fp); // Ler cada termo da matriz salva no arquivo e converte para int
int ** set_matriz(int **matriz,FILE *fp, int L, int C);  // seta uma matriz LxC com os valores salvo em um arquivo
int** aloc_matriz(int L,int C); // aloca dinamicamente memória para uma matriz LxC
void print_matriz(int **matriz,int L,int C); // printa uma matriz na tela de dimensão LxC
void print_vetor(int* vetor, int N); // printa na tela os valores de um vetor de N posições
void* trata_linha(void* entrada); // Função da Thread que analisará se a linha é ou não válida

void extrai_linha(int** matriz, int linha, int* vetor_linha); // extrai uma linha específica de uma matriz
void extrai_coluna(int** matriz, int coluna, int* vetor_coluna); // extrai uma coluna específica de uma matriz
void extrai_submatriz(int** matriz, int n, int m, int** submatriz); // extrai um submatriz 3x3 de uma matriz




int resultado[27];


typedef struct
{
	int** matriz_sudoku;
	int linha;
}Param_linha;

typedef struct
{

	int** matriz_sudoku;
	int coluna;
}Param_coluna;





int main(){

	FILE *fp_sudoku; // Descritor de arquivo do Sudoku
	pthread_t dthread_linha[9]; // Descritores das 9 Threads q analisarão as linhas da matriz
	
	int** matriz_sudoku; // Matriz que armazenará os dados lidos do arquivo	
	matriz_sudoku = aloc_matriz(9,9); // aloca espaço em memória para a matriz_sudoku

	fp_sudoku = fopen("./sudokus/6.txt", "r"); // abrindo o arquivo para leitura
	matriz_sudoku = set_matriz(matriz_sudoku, fp_sudoku, 9,9);// passando os dados do arquivo para a matriz
	
	Param_linha* l; // Struct que será passada como parâmetros para as Threads de validação de linha
	l = (Param_linha*) malloc(sizeof(Param_linha)); // Alocando memória para a Struct


	l->matriz_sudoku = matriz_sudoku; // passando a matriz_sudoku para a struct
	
	// chamando as threads para tratar as linhas
	for(int i = 0; i<9; i++)
	{
		l->linha = i; // passando a linha que a Thread irá avaliar

		pthread_create(&dthread_linha[i], NULL,trata_linha, (void*) l); // chamando uma Thread para analisar uma linha
		usleep(5000); // esperando que a Thread leia os parâmetros antes de chamar a próx Thread
	}

	
	// Esperando todas as Threads terminarem.
	for(int i = 0; i<9; i++)
	{
		pthread_join(dthread_linha[i], NULL);
	}

	return 0;
}





void extrai_linha(int** matriz, int linha, int* vetor_linha)
{
	for(int i = 0; i<9; i++)
	{
		vetor_linha[i] = matriz[linha][i];
		printf("vetor_linha[i] = %d\n", vetor_linha[i]);
	}
}




void* trata_linha(void* in)
{		
	int* vetor_linha;
	int num_linha;
	int sum = 0;

	// arrumando parâmetros de entrada
	Param_linha *entrada;
	entrada = (Param_linha*) malloc(sizeof(Param_linha));
	entrada = (Param_linha*) in;

	printf("------------------ thread%d----------------\n", entrada->linha);

	vetor_linha = (int *) malloc(9*sizeof(int));

	extrai_linha(entrada->matriz_sudoku, entrada->linha, vetor_linha);
	
	for(int i = 0; i<9; i++)
	{
		sum += vetor_linha[i];
	}

	if(sum == 45)
	{
		printf("linha válida\n");
		resultado[entrada->linha] = 109;

	}else{
		printf("linha inválida\n");
		resultado[entrada->linha] = 0;
	}


}



int ler_termo(FILE *fp)
{
	char A;
	A = getc(fp);
	
	if(A == '\r'){
		A = getc(fp);
	}
	
	if(A == '\n'){
		A = getc(fp);
	}

	if(A == ' '){
		A = getc(fp);
	}

		
	return atoi(&A);
}


int** set_matriz(int **matriz,FILE *fp,int L,int C)
{
	for(int i = 0; i < L; i++)
	{
		for(int j = 0; j<C; j++)
		{
			matriz[i][j] = ler_termo(fp);
		}
	}
	return matriz;
}


int** aloc_matriz(int L,int C)
{
	int **Matriz;
	Matriz = (int **) malloc(L*sizeof(int *));

	for(int i = 0; i < L; i++)
	{
		*(Matriz + i) = (int*) malloc(C*sizeof(int));

	}
	return Matriz;
}

void print_matriz(int **matriz,int L,int C)
{
	for(int i = 0; i < L; i++)
	{
		for(int j = 0; j<C; j++)
		{
			printf("matriz[%d][%d] = %d\n", i,j,matriz[i][j]);
		}
	}
}

void print_vetor(int* vetor, int N)
{
	for(int i = 0; i<N; i++)
	{
		printf("vetor[%d] = %d\n", i, vetor[i]);
	}

}



