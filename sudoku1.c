#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


int ler_termo(FILE *fp); // Ler cada termo da matriz salva no arquivo e converte para int
int ** set_matriz(int **matriz,FILE *fp, int L, int C);  // seta uma matriz LxC com os valores salvo em um arquivo
int** aloc_matriz(int L,int C);
void print_matriz(int **matriz,int L,int C);
void print_vetor(int* vetor, int N); // printa na tela os valores de um vetor de N posições
void* trata_linha(void* entrada);

void extrai_linha(int** matriz, int linha, int* vetor_linha);
void extrai_coluna(int** matriz, int coluna, int* vetor_coluna);
void extrai_submatriz(int** matriz, int n, int m, int** submatriz);




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

	pthread_t dthread_linha[9];
	FILE *fp_sudoku;
	int** matriz_sudoku;		
	matriz_sudoku = aloc_matriz(9,9);

	fp_sudoku = fopen("./sudokus/6.txt", "r");
	matriz_sudoku = set_matriz(matriz_sudoku, fp_sudoku, 9,9);	
	
	printf("oi1\n");
	
	Param_linha* l;
	l = (Param_linha*) malloc(sizeof(Param_linha));

	printf("oi2\n");
	l->matriz_sudoku = matriz_sudoku;
	printf("oi3\n");
	// chamando as threads para tratar as linhas
	for(int i = 0; i<9; i++)
	{
		l->linha = i;

		pthread_create(&dthread_linha[i], NULL,trata_linha, (void*) l);
		usleep(5000);
	}

	
	
	for(int i = 0; i<9; i++)
	{
		pthread_join(dthread_linha[i], NULL);
	}

	

	printf("------------- main -----------\n");
	printf("%d\n", resultado[3]);




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



