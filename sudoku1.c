#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>


int ler_termo(FILE *fp); // Ler cada termo da matriz salva no arquivo e converte para int
int ** set_matriz(int **matriz,FILE *fp, int L, int C);  // seta uma matriz LxC com os valores salvo em um arquivo
int** aloc_matriz(int L,int C); // aloca dinamicamente memória para uma matriz LxC
void print_matriz(int **matriz,int L,int C); // printa uma matriz na tela de dimensão LxC
void print_vetor(int* vetor, int N); // printa na tela os valores de um vetor de N posições
void* trata_linha(void* entrada); // Função da Thread que analisará se a linha é ou não válida
void* trata_coluna(void* in); // Função da Thread que analisará se a coluna é ou não valida
void* trata_matriz(void* entrada);// Função da Thread que analizará se a submatriz 3x3 é ou não válida

void extrai_linha(int** matriz, int linha, int* vetor_linha); // extrai uma linha específica de uma matriz
void extrai_coluna(int** matriz, int coluna, int* vetor_coluna); // extrai uma coluna específica de uma matriz
int** desmembra_matriz(int** matriz, int m, int n);// extrai um submatriz 3x3 de uma matriz




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

typedef struct
{
	int** matriz3x3;
	int n_matriz;
}Param_matriz;





int main(){

	FILE *fp_sudoku; // Descritor de arquivo do Sudoku
	pthread_t dthread_linha[9]; // Descritores das 9 Threads q analisarão as linhas da matriz
	pthread_t dthread_coluna[9]; // Descritores das 9 Threads q analisarão as colunas da matriz
	pthread_t dthread_submatriz[9]; // Descritores das 9 Threads que analisarão as submatrizes 3x3

	int** matriz_sudoku; // Matriz que armazenará os dados lidos do arquivo
	matriz_sudoku = aloc_matriz(9,9); // aloca espaço em memória para a matriz_sudoku

	fp_sudoku = fopen("./sudokus/7.txt", "r"); // abrindo o arquivo para leitura
	matriz_sudoku = set_matriz(matriz_sudoku, fp_sudoku, 9,9);// passando os dados do arquivo para a matriz

	Param_linha* l; // Struct que será passada como parâmetros para as Threads de validação de linha
	l = (Param_linha*) malloc(sizeof(Param_linha)); // Alocando memória para a Struct

	Param_coluna* c; // Struct que será passada como parâmetros para as Threads de validação de coluna
	c = (Param_coluna*) malloc(sizeof(Param_coluna)); // Alocando memória para a Struct


	l->matriz_sudoku = matriz_sudoku; // passando a matriz_sudoku para a struct linha
	c->matriz_sudoku = matriz_sudoku; // passando a matriz_sudoku para a struct coluna


	// chamando as threads para tratar as linhas
	for(int i = 0; i<9; i++)
	{
		l->linha = i; // passando a linha que a Thread irá avaliar

		pthread_create(&dthread_linha[i], NULL,trata_linha, (void*) l); // chamando uma Thread para analisar uma linha
		usleep(5000); // esperando que a Thread leia os parâmetros antes de chamar a próx Thread
	}

	for(int i = 0; i<9; i++)
	{
		c->coluna = i; // passando a linha que a Thread irá avaliar

		pthread_create(&dthread_coluna[i], NULL,trata_coluna, (void*) c); // chamando uma Thread para analisar uma linha
		usleep(5000); // esperando que a Thread leia os parâmetros antes de chamar a próx Thread
	}

	int** matriz3x3;
	Param_matriz* m;
	m = (Param_matriz*) malloc(sizeof(Param_matriz)); // Alocando memória para a Struct

	int cont_matriz = 0;

	for(int i = 0; i<9; i+=3)
	{
		for(int j = 0; j<9; j+=3)
		{
			matriz3x3 = desmembra_matriz(matriz_sudoku, i,j);
			m->n_matriz = cont_matriz;
			m->matriz3x3 = matriz3x3;

			pthread_create(&dthread_submatriz[cont_matriz], NULL, trata_matriz, (void*) m);
			pthread_join(dthread_submatriz[cont_matriz], NULL);

			cont_matriz++;
		}
	}


	// Esperando todas as Threads terminarem.
	for(int i = 0; i<9; i++)
	{
		pthread_join(dthread_linha[i], NULL);
		pthread_join(dthread_coluna[i], NULL);
	}

	// Avaliando o Resultado do vetor resultado

	int soma_resultado = 0;

	for(int i=0; i<27; i++)
	{
		if(i < 9)
		{
		    if(resultado[i] == 0)
			{
				printf("Existe um erro na linha %d\n", (i+1));
			}


		}else if(i < 18)
		{
			 if(resultado[i] == 0)
			{
				printf("Existe um erro na coluna %d\n", (i-8));
			}


		}else
		{
			if(resultado[i] == 0)
			{
				printf("Existe um erro na matriz %d\n", (i-17));
			}
		}

		soma_resultado += resultado[i];

	}

	if(soma_resultado == 27)
	{
		printf("Parabéns! A resolução do Sudoku está correta!\n");
	}

	return 0;
}


void* trata_matriz(void* in)
{
	Param_matriz* entrada;
	entrada = (Param_matriz*) malloc(sizeof(Param_matriz));
	entrada = (Param_matriz*) in;
	int soma = 0;

	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			soma+=entrada->matriz3x3[i][j];
		}
	}

	if(soma == 45)
	{
		resultado[18 + entrada->n_matriz] = 1;
	}else{
		resultado[18+ entrada->n_matriz] = 0;
	}

}


int** desmembra_matriz(int** matriz, int m, int n)
{
	int** matriz3x3;
	matriz3x3 = aloc_matriz(3,3);
	for(int i = m; i <(m+3); i++)
	{
		for(int j = n; j<(n+3); j++)
		{
			matriz3x3[i-m][j-n] = matriz[i][j];
		}
	}

	return matriz3x3;
}






void extrai_linha(int** matriz, int linha, int* vetor_linha)
{
	for(int i = 0; i<9; i++)
	{
		vetor_linha[i] = matriz[linha][i];
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

	vetor_linha = (int *) malloc(9*sizeof(int));

	extrai_linha(entrada->matriz_sudoku, entrada->linha, vetor_linha);

	for(int i = 0; i<9; i++)
	{
		sum += vetor_linha[i];
	}

	if(sum == 45)
	{
		resultado[entrada->linha] = 1;
	}else{
		resultado[entrada->linha] = 0;
	}
}


void extrai_coluna(int** matriz, int coluna, int* vetor_coluna)
{
	for(int i = 0; i<9; i++)
	{
		vetor_coluna[i] = matriz[i][coluna];
	}
}




void* trata_coluna(void* in)
{
	int* vetor_coluna;
	int num_coluna;
	int sum = 0;

	// arrumando parâmetros de entrada
	Param_coluna *entrada;
	entrada = (Param_coluna*) malloc(sizeof(Param_coluna));
	entrada = (Param_coluna*) in;

	vetor_coluna = (int *) malloc(9*sizeof(int));

	extrai_coluna(entrada->matriz_sudoku, entrada->coluna, vetor_coluna);

	for(int i = 0; i<9; i++)
	{
		sum += vetor_coluna[i];
	}

	if(sum == 45)
	{
		resultado[9 + entrada->coluna] = 1;
	}else{
		resultado[9 + entrada->coluna] = 0;
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
