#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "omp.h"
#define WIDTH 12000
#define HEIGHT 6000
#define DIMENSON 5

typedef struct{
  unsigned char r;
  unsigned char g;
  unsigned char b;  
}pixel;

typedef struct{
  int pthreadId;
  int pthreadNum;
  int i;
  int j;
}Arguments;


pixel sourceImage[HEIGHT][WIDTH];
pixel targetImage[HEIGHT][WIDTH];

void readFile(char *argv){
  FILE *fp;

  if((fp = fopen(argv, "rb")) == NULL){
    printf("File couldn't be read\n");
    exit(1);
  }

  fread(sourceImage, sizeof(sourceImage), 1, fp);
  fclose(fp);
}

void writeFile(){
  FILE *fp2;

  if((fp2 = fopen("/home/matthew/FIIT/4.semester/APS/zadanie/results.data", "wb")) == NULL){
    printf("Couldn't write teh file.\n");
    exit(1);
  }else{
    fwrite(targetImage, sizeof(targetImage), 1, fp2 );
    fclose(fp2);   
    printf("Sucess. Stored in results.data\n");
  }
}

unsigned char checkValue(int a){
  // printf("%d ", pixel);
  if(a>255){
    return 255;
  }else if(a<0){
    return 0;
  }else{
    return a;
  }
}

void convolutionFilter(int i, int j){
  int k,l,matrixLeft,matrixRight,m,n;
  int cumR, cumG, cumB;
  
  // int kernelMatrix[DIMENSON][DIMENSON] = {
  //   0,0,-1,0,0,
  //   0,-1,-2,-1,0,
  //   -1,-2,16,-2,-1,
  //   0,-1,-2,-1,0,
  //   0,0,-1,0,0
  // };

  // int kernelMatrix[DIMENSON][DIMENSON] = {
  //   0,0,0,0,0,
  //   0,0,0,0,0,
  //   0,0,1,0,0,
  //   0,0,0,0,0,
  //   0,0,0,0,0
  // };

  int kernelMatrix[DIMENSON][DIMENSON] = {
    1,4,6,4,1,
    4,16,24,16,4,
    6,24,36,24,6,
    4,16,24,16,4,
    1,4,6,4,1
  };

  matrixLeft = -DIMENSON/2;
  matrixRight = DIMENSON/2;

  cumR = 0;
  cumG = 0;
  cumB = 0;
  m = 0;

  for(k=matrixLeft;k<=matrixRight;k++){
    n = 0;
    for(l=matrixLeft;l<=matrixRight;l++){
      if((i+k<0) || (i+k>=HEIGHT)){
        n++;
        continue;
      }else if((j+l<0) || (j+l>=WIDTH)){
        n++;
        continue;
      }
      cumR += (int)sourceImage[i+k][j+l].r * kernelMatrix[m][n];
      cumG += (int)sourceImage[i+k][j+l].g * kernelMatrix[m][n];
      cumB += (int)sourceImage[i+k][j+l].b * kernelMatrix[m][n];

      n++;
    }
    m++;
  }
  targetImage[i][j].r = checkValue(cumR/256);
  targetImage[i][j].g = checkValue(cumG/256);
  targetImage[i][j].b = checkValue(cumB/256);
}

void *create(void *ptr){

  Arguments* data = (Arguments*) ptr;
  int threadsNum = ((Arguments*)ptr)->pthreadNum;
  int threadsId = ((Arguments*)ptr)->pthreadId;

  

  printf("num %d id %d\n", threadsNum, threadsId);

   for(int i=0;i<HEIGHT;i++){ 
      for(int j=(WIDTH/threadsNum)*threadsId;j<(WIDTH/threadsNum)*(threadsId+1)-1;j++){
        convolutionFilter(i,j);
      }
    }
}

void help(){
  printf("Convoluted FILTER (c) Matúš Brandýs\n");
  printf("Usage: ./filter.out [options] file\n");
  printf("Options:\n");
  printf("  --help; -h \t\tDisplay this information.\n");
  printf("  --pthreads [num] [file]\tRun program using [num] numbers threads and [file] file.\n\t\t\tRun program using parallelism.\n");
  printf("  --basic \t\tRun sequence program.\n");
}

int isNumber(char number[])
{
    int i = 0;
    //checking for negative numbers
    if (number[0] == '-'){
      printf("%s is negative number\n", number);
      return 0;
    }
    for (; number[i] != 0; i++)
    {
      if (!isdigit(number[i])){
        printf("%s not a number\n", number);
        return 0;
      }
    }
    return 1;
}

void basic(char *file){

  printf("program running sequentialy\n");

  readFile(file);

  for(int i=0;i<HEIGHT;i++){ 
    for(int j=0;j<WIDTH;j++){
      convolutionFilter(i,j);
    }
  }
}

void pthreads(int threadsNum, char* file){
  // int threadsNum = atoi(argv[1]);
  printf("running  parallelism");
  pthread_t process[threadsNum];

  readFile(file);

  for (int a=0;a<threadsNum;a++){
    Arguments *arguments;
    arguments = (Arguments*)malloc(sizeof(Arguments));
    arguments->pthreadId = a;
    arguments->pthreadNum = threadsNum;
    pthread_create( &process[a], NULL, &create,(void*)arguments); 
  }
  
  for (int i=0;i<threadsNum;i++)
    pthread_join( process[i], NULL);

  writeFile();
}

int main(int argc, char **argv){

  int input = 0;

  while(input < argc){
    if((strcmp(argv[input], "-h") == 0) ||  (strcmp(argv[input], "--help") == 0)){
      help();
      exit(0);
    }else if(strcmp(argv[input], "--basic") == 0){
      if(argv[input+1] != NULL){
        basic(argv[input+1]);
        writeFile();
        exit(0);
      }else{
        printf("No file specified\n");
        exit(0);
      }
    }else if(strcmp(argv[input], "--pthreads") == 0){
      char *threadsNum = argv[input+1];
      char *file = argv[input+2];
      printf("presiel\n");

      if (!isNumber(threadsNum)){
        printf("Please specify threads numbers. See -h or --help");
        exit(0);
      }else{
        pthreads(atoi(threadsNum), file);
        writeFile();
        exit(0);
      }
    }else if (argc == 1){
        printf("No parameters specified. Use --help or -h\n");
        exit(0);
    }else if(input == argc){
        printf("Unknown parameters. Use --help or -h\n");
        exit(0);
    }
    input++;
  }
  return 0;
}