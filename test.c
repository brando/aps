#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#define WIDTH 21600
#define HEIGHT 21600

typedef struct{
  unsigned char r;
  unsigned char g;
  unsigned char b;  
}pixel;

pixel sourceImage[WIDTH][HEIGHT];
pixel targetImage[WIDTH][HEIGHT];

void readFile(char *argv){
  FILE *fp;

  if((fp = fopen(argv, "rb")) == NULL){
    printf("Subor sa nepodarilo otvorit.\n");
    exit(1);
  }

  fread(sourceImage, sizeof(sourceImage), 1, fp);
  fclose(fp);

  // for(i=0;i<WIDTH;i++){
  //   for(j=0;j<HEIGHT;j++){
  //     targetImage[i][j].r = sourceImage[i][j].r-10;
  //     targetImage[i][j].g = sourceImage[i][j].g-10;
  //     targetImage[i][j].b = sourceImage[i][j].b-10;
  //   }
  // }
}

void writeFile(){
  FILE *fp2;

  if((fp2 = fopen("/home/matthew/FIIT/4.semester/APS/zadanie/restuls.data", "wb")) == NULL){
    printf("Subor sa nepodarilo otvorit.\n");
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

void convolutionFilter(){
  int i,j,k,l,matrixLeft,matrixRight,m,n;
  // int cumR, cumG, cumB;
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

  // printf("%d %d\n", matrixLeft, matrixRight );

  //  for(i=0;i<WIDTH;i++){
  //   for(j=0;j<HEIGHT;j++){
  //     targetImage[i][j].r = 0;
  //     targetImage[i][j].g = 0;
  //     targetImage[i][j].b = 0;
  //   }
  // }
 
  for(int i=0;i<WIDTH;i++){
    for(int j=0;j<HEIGHT;j++){
      int cumR = 0;
      int cumG = 0;
      int cumB = 0;
      int m = 0;
      for(int k=matrixLeft;k<=matrixRight;k++){
        n = 0;
        for(int l=matrixLeft;l<=matrixRight;l++){
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
  }
}

int main(int argc, char **argv){

  FILE *fp;
  int c;

  readFile(argv[1]);
  convolutionFilter();
  writeFile();

  return 0;
}