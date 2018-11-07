#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

int gera_aleatorio(int, int);
void* estado_ajudante(void* ent);
void* estado_alunos(void*);



typedef struct{
  int cont_ajudas;
  int pos_cadeira;
  int id;
}Param_alunos;

typedef struct{
  int o_cadeira;
}Param_ajudante;

int main(){

  int num_alunos, num_cadeiras;
  Param_alunos *alunos;
  Param_ajudante *ajudante;

  alunos = (Param_alunos*)malloc(sizeof(Param_alunos));
  ajudante = (Param_ajudante*)malloc(sizeof(Param_ajudante));

  num_alunos = gera_aleatorio(3, 40);
  num_cadeiras = num_alunos/2;

  pthread_t dthread_alunos[num_alunos];
  pthread_t dthread_ajudante;

  for(int i=0;i<num_alunos;i++){
    alunos->id = i;
    alunos->cont_ajudas = 0;
    pthread_create(&dthread_alunos[i], NULL,estado_alunos, (void*)alunos);
    usleep(500);
  }

  for(int i=0;i<num_alunos;i++){
    pthread_join(dthread_alunos[i], NULL);
  }

  printf("Fim!!!\n");

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
  alunos = (Param_alunos*)malloc(sizeof(Param_alunos));
  alunos = (Param_alunos*)ent;

  printf("Aluno: %d\nNumero de ajudas: %d\n", alunos->id, alunos->cont_ajudas);
  pthread_exit(0);
}

void* estado_ajudante(void* Param_ajudante){
  pthread_exit(0);
}
