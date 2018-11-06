#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

void estado_ajudante(void* Param_alunos);
void estado_alunos(void* Param_ajudante);

typedef struct{
  int cont_ajudas;
  int pos_cadeira;
}Param_alunos;

typedef struct{
  int o_cadeira;
}Param_ajudante;

int main(){

  time_t t;
  srand((unsigned) time(&t));

  int num_alunos, num_cadeiras;

  num_alunos = (rand() % 38) + 3;
  num_cadeiras = num_alunos/2;

  pthread_t dthread_alunos[num_alunos];
  pthread_t dthread_ajudante;

  return(0);
}

void estado_alunos(void* Param_alunos){

}

void estado_ajudante(void* Param_ajudante){

}
