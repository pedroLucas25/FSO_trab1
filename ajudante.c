#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main () {

   time_t t;
   srand((unsigned) time(&t));

   int num_alunos, num_cadeiras;

   num_alunos = (rand() % 38) + 3;
   num_cadeiras = num_alunos/2;

   printf("%d\n%d\n", num_alunos, num_cadeiras);


   return(0);
}
