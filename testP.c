#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int pole[9] = {1,2,3,4,5,6,7,8,9};
int vysledok[9];
int stop = 0;

void *processF(void *ptr){

  int i = (int*)ptr;
  int medzisucet;
  int j,k;
  for(k=0;k<3;k++){
    medzisucet = 0;
    for(j=0;j<3;j++){
      if(pole[i*3+k+j] != NULL){
        medzisucet += pole[i*3+k+j];
      }
      else{
        medzisucet = 25;
      }
    }
    sleep(5);
  vysledok[k+i*3] = medzisucet;
  }
}

int main(int argc, char **argv){

  int i;
  if((strcmp(argv[1], "-h") == 0) ||  (strcmp(argv[1], "--help") == 0)){
    printf("help\n");
    exit(0);
  }
  
  int num = atoi(argv[1]);
  printf("%d\n", num);

  pthread_t process[num];

  printf("zaciatok\n");
  for (i=0;i<num;i++) pthread_create( &process[i], NULL, &processF,(void*)i); 
  // sleep(1);
  printf("koniec\n");

  for (i=0;i<num;i++)
    pthread_join( process[i], NULL);

  for (i=0;i<9;i++){
    printf("%d\n", vysledok[i]);
  }

  return 0;
}