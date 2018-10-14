#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "omp.h"
#define WIDTH 21600
#define HEIGHT 21600

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


pixel sourceImage[WIDTH][HEIGHT];
pixel targetImage[WIDTH][HEIGHT];

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

  if((fp2 = fopen("/home/matthew/FIIT/4.semester/APS/zadanie/restuls.data", "wb")) == NULL){
    printf("Couldn't write teh file.\n");
    exit(1);
  }else{
    fwrite(targetImage, sizeof(targetImage), 1, fp2 );
    fclose(fp2);   
    printf("Sucess. Stored in restuls.data\n");
  }
}

char checkValue(int a){
  // printf("%d ", pixel);
  if(a>=255){
    return 255;
  }else if(a<=0){
    return 0;
  }else{
    return a;
  }
}

void convolutionFilter(int i, int j){
  int k,l,matrixLeft,matrixRight,m,n;
  int cumR, cumG, cumB;
  // int kernelMatrix[3][3] = {
  //   +1, 0, 0,
  //   0, 0, 0,
  //   0, 0, -1
  // };

  int kernelMatrix[3][3] = {
    0, -1, 0,
    -1, 6, -1,
    0, -1, 0
  };

  matrixLeft = -3/2;
  matrixRight = 3/2;

  cumR = 0;
  cumG = 0;
  cumB = 0;
  m = 0;

  for(k=matrixLeft;k<=matrixRight;k++){
    n = 0;
    for(l=matrixLeft;l<=matrixRight;l++){
      if((i+k<0) || (i+k>=WIDTH)){
        n++;
        continue;
      }else if((j+l<0) || (j+l>=HEIGHT)){
        n++;
        continue;
      }
      // printf("%d %d %d\n", sourceImage[i+k][j+l].r, sourceImage[i+k][j+l].g, sourceImage[i+k][j+l].b);
      cumR += (int)sourceImage[i+k][j+l].r * kernelMatrix[m][n];
      cumG += (int)sourceImage[i+k][j+l].g * kernelMatrix[m][n];
      cumB += (int)sourceImage[i+k][j+l].b * kernelMatrix[m][n];
      
      // printf("tu: %d %d %d\n", cumR, cumG, cumB);
      // cumR += sourceImage[i+k][j+l].r * 1;
      // cumG += sourceImage[i+k][j+l].g * 1;
      // cumB += sourceImage[i+k][j+l].b * 1;
      n++;
    }
    m++;
  }
  // cumR = ;
  // cumG = ;
  // cumB = ;
  // printf("%d %d %d\n", cumR, cumG, cumB);
  targetImage[i][j].r = checkValue(cumR);
  targetImage[i][j].g = checkValue(cumG);
  targetImage[i][j].b = checkValue(cumB);
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
  printf("  --pthreads [num]\tRun program using [num] numbers threads.\n\t\t\tRun program using parallelism.\n");
  printf("  --basic \t\tRun sequence program.\n");
}

void basic(char *file){

  printf("program running sequentialy\n");

  int i,j;

  readFile(file);

  for(int i=0;i<WIDTH;i++){ 
    for(int j=0;j<HEIGHT;j++){
      convolutionFilter(i,j);
    }
  }
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

      exit(0);
    }else{
      printf("here\n");
      if(input == argc){
        printf("Unknown parameters. Use --help or -h\n");
        exit(0);
      }
    }
    input++;
  }
  
  
  FILE *fp;
  int c;
  int threadsNum = atoi(argv[1]);
  pthread_t process[threadsNum];

  readFile(argv[2]);

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

  return 0;
}