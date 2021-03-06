#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

/* Chat




*/

/* Itens a serem desenvolvidos

- Gerenciar a fila com semáforos na função estado_alunos() e estado_ajudante()

- Gerenciar o sleep do estado_ajudante()



*/

int gera_aleatorio(int, int);
void* estado_ajudante(void*);
void* estado_alunos(void*);
void incrementa_fila(int*);
void retira_da_fila(int*);
int conta_fila(int*);

typedef struct{
  int cont_ajudas; // se colocar = 3 não compila
  int pos_cadeira;
  int id;
}Param_alunos;

typedef struct{
  int o_cadeira;
}Param_ajudante;

int *fila;

sem_t sem_fila, sem_ajudante;

int main(){

  int num_alunos, num_cadeiras;
  Param_alunos *alunos;
  Param_ajudante *ajudante;

  alunos = (Param_alunos*)malloc(sizeof(Param_alunos));
  ajudante = (Param_ajudante*)malloc(sizeof(Param_ajudante));

  num_alunos = gera_aleatorio(3, 40);
  num_cadeiras = num_alunos/2;

  fila = (int*)malloc(num_cadeiras);

  sem_init(&sem_fila, 0, num_cadeiras); // Iniciando semáforo de tamanho num_alunos para gerenciar a fila
  sem_init(&sem_ajudante, 0, 1);// Iniciando semáforo de tamnho 1 para gerenciar se o Monitor estará acordado ou dormindo

  for(int i=0;i<num_cadeiras;i++){
    printf("%d\n", fila[i]);
  }



  pthread_t dthread_alunos[num_alunos];
  pthread_t dthread_ajudante;

  pthread_create(&dthread_ajudante, NULL, estado_ajudante, (void*)ajudante);

  for(int i=0;i<num_alunos;i++){
    alunos->id = i;
    pthread_create(&dthread_alunos[i], NULL,estado_alunos, (void*)alunos);
    usleep(100);
  }

  for(int i=0;i<num_alunos;i++){
    pthread_join(dthread_alunos[i], NULL);
  }

  printf("Todos os alunos foram atendidos, Monitor liberado!\n");
  pthread_join(dthread_ajudante, NULL);

  printf("FIM\n");

  return(0);
}

int gera_aleatorio(int inter_a, int inter_b){
  time_t t;
  srand((unsigned) time(&t));
  int resultado;
  resultado = (rand() % (inter_b-inter_a+1)) + inter_a;
  return resultado;
}

void* estado_alunos(void* ent){
  Param_alunos* alunos;
  Param_alunos* aux;
  aux = (Param_alunos*) ent;
  alunos = (Param_alunos*)malloc(sizeof(Param_alunos));
  ent = (Param_alunos*) ent;

  alunos->pos_cadeira = aux->pos_cadeira;
  alunos->id = aux->id;
  alunos->cont_ajudas = 0;

  int value_sem_ajudante;

  sem_getvalue(&sem_ajudante, &value_sem_ajudante);

  if(value_sem_ajudante == 0) // Verifica
    {
      printf("Estudante %d acordando o Monitor\n", alunos->id);
      sem_post(&sem_ajudante);

      sem_wait(&sem_fila);
      // insere(alunos->id);
      // printf("Estudante sentado no banco %d", cont_fila(fila));

    }

  printf("Estudante %d estudando!", alunos->id);
  sleep(1);



  pthread_exit(0);
}

void* estado_ajudante(void* ent){

  Param_ajudante *ajudante;

  ajudante = (Param_ajudante*)malloc(sizeof(Param_ajudante));

  int value_sem_ajudante;

  while(1){

    sem_getvalue(&sem_ajudante, &value_sem_ajudante);

    if(value_sem_ajudante == 0)
      printf("Ajudante Dormindo");
    if(value_sem_ajudante != 0)
      printf("Erro!");


    printf("Ajudante Acordado!\n");



    if(value_sem_ajudante == 0) printf("Ajudante Dormindo");

}

  pthread_exit(0);
}

void incrementa_fila(int id){

}
