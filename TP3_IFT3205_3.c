/*------------------------------------------------------*/
/* Prog    : Tp3_IFT3205-2-2.c                          */
/* Auteur  : Élie Leblanc, Justin Veilleux              */
/* Date    : --/--/2010                                 */
/* version :                                            */ 
/* langage : C                                          */
/* labo    : DIRO                                       */
/*------------------------------------------------------*/
// elie.leblanc@umontreal.ca justin.veilleux@umontreal.ca
/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo3.h"
#include "fonctions.c"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN  "barbara512_Degraded"
#define NAME_IMG_OUT  "barbara512_Undegraded"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/


/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int length,width;
  char BufSystVisuImg[100];

  //Lecture Image 
  float** img=LoadImagePgm(NAME_IMG_IN,&length,&width);
  float ** img_copy = fmatrix_allocate_2d(length, width);
  fmatrix_move(length, width, img, img_copy);

  float ** matR = fmatrix_allocate_2d(length, width);
  float ** matI = fmatrix_allocate_2d(length,width);
  float max_norm;
  for (int iter = 0;iter < 15;iter++) {
      fmatrix_zero(length, width, matI);
      fmatrix_move(length, width, img_copy, matR);
      FFTDD(matR, matI, length, width);
      if (iter == 0) {
          max_norm = norm(matR[0][0], matI[0][0]);

      }
      float lambda = 0.04 / pow(2, iter);
      float norm_min = max_norm * lambda;
      threshold(matR, matI, length, width, norm_min);
      IFFTDD(matR, matI, length, width);

      char buffer[100];
      sprintf(buffer, "barbara512_filtered%d", iter);
      SaveImagePgm(buffer, matR, length, width);
      
      float threshold_white = 255.0;
  
      for (int i = 0;i < length;i++) {
          for (int j = 0;j < width;j++) {
              if (img[i][j] >= threshold_white) {
                  img_copy[i][j] = matR[i][j];
              }
          }
      }
  }
  
  SaveImagePgm(NAME_IMG_OUT, img_copy, length, width);
  /* for (i = 0; i < length; i++) { */
  /*     for (j = 0; j < width; j++) { */
  /*        MatriceImg2R[i+(int)(length*1.5)][j+(int)(width*1.5)]=MatriceImg1R[i][j]; */
  /*        MatriceImg2I[i+(int)(length*1.5)][j+(int)(width*1.5)]=MatriceImg1I[i][j]; */
  /*      } */
  /*  } */
  /*  CenterImg(MatriceImg2R, length*4, width*4); */
  /*  CenterImg(MatriceImg2I, length*4, width*4); */
  /* IFFTDD(MatriceImg2R,MatriceImg2I, length*4, width*4); */
  /* //Sauvegarde */
  /*  //Multiplication par 16 pour conserver l'énergie */
  /* Mult(MatriceImg2R,16, length*4, width*4); */
  /* SaveImagePgm(NAME_IMG_OUT,MatriceImg2R,length*4,width*4); */

  /* //Commande systeme: VISU */
  /* strcpy(BufSystVisuImg,NAME_VISUALISER); */
  /* strcat(BufSystVisuImg,NAME_IMG_OUT); */
  /* strcat(BufSystVisuImg,".pgm&"); */
  /* printf(" %s",BufSystVisuImg); */
  /* system(BufSystVisuImg); */



  //==End=========================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


