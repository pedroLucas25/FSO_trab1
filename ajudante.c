#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct{
  int cont_ajudas;
}Param_alunos;


int main () {

   time_t t;
   srand((unsigned) time(&t));

   int num_alunos, num_cadeiras;

   num_alunos = (rand() % 38) + 3;
   num_cadeiras = num_alunos/2;

   pthread_t dthread_alunos[num_alunos];
   pthread_t dthread_ajudante;


   return(0);
}
