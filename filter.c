#include <stdio.h>
#include <stdlib.h>
#define WIDTH 1960
#define HEIGHT 1960

typedef struct{
  char r;
  char g;
  char b;  
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
    fwrite(targetImage, 1, sizeof(targetImage), fp2 );
    fclose(fp2);   
    printf("Sucess. Stored in restuls.data\n");
  }
}

void convolutionFilter(){
  int i,j,k,l,matrixLeft,matrixRight,m,n;
  char cumR, cumG, cumB;
  int kernelMatrix[3][3] = {
    +1, 0, 0,
    0, 0, 0,
    0, 0, -1
  };

  // int kernelMatrix[3][3] = {
  //   0, -1, 0,
  //   -1, 5, -1,
  //   0, -1, 0
  // };

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

  for(i=0;i<WIDTH;i++){
    for(j=0;j<HEIGHT;j++){
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
          cumR += sourceImage[i+k][j+l].r * kernelMatrix[m][n];
          cumG += sourceImage[i+k][j+l].g * kernelMatrix[m][n];
          cumB += sourceImage[i+k][j+l].b * kernelMatrix[m][n];
          
          // cumR += sourceImage[i+k][j+l].r * 1;
          // cumG += sourceImage[i+k][j+l].g * 1;
          // cumB += sourceImage[i+k][j+l].b * 1;
          n++;
        }
        m++;
      }
      targetImage[i][j].r = cumR;
      targetImage[i][j].g = cumG;
      targetImage[i][j].b = cumB;
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