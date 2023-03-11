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
#define NAME_IMG_IN  "lena128"
#define NAME_IMG_OUT "lena512_InterpolPPV"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/


/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j;
  int length,width;
  char BufSystVisuImg[100];

  //Lecture Image 
  float** MatriceImg1R=LoadImagePgm(NAME_IMG_IN,&length,&width);
  float** MatriceImg1I=fmatrix_allocate_2d(length,width);
  float** MatriceImg2R=fmatrix_allocate_2d(length*4,width*4);
  float** MatriceImg2I=fmatrix_allocate_2d(length*4,width*4);

   fmatrix_zero(length, width, MatriceImg1I);
   fmatrix_zero(length*4, width*4, MatriceImg2R);
   fmatrix_zero(length*4, width*4, MatriceImg2I);

  //Interpolation
  FFTDD(MatriceImg1R, MatriceImg1I, length, width);
   CenterImg(MatriceImg1R, length, width);
   CenterImg(MatriceImg1I, length, width);

  for (i = 0; i < length; i++) {
      for (j = 0; j < width; j++) {
         MatriceImg2R[i+(int)(length*1.5)][j+(int)(width*1.5)]=MatriceImg1R[i][j];
         MatriceImg2I[i+(int)(length*1.5)][j+(int)(width*1.5)]=MatriceImg1I[i][j];
       }
   }
   CenterImg(MatriceImg2R, length*4, width*4);
   CenterImg(MatriceImg2I, length*4, width*4);
  IFFTDD(MatriceImg2R,MatriceImg2I, length*4, width*4);
  //Sauvegarde
   //Multiplication par 16 pour conserver l'énergie
  Mult(MatriceImg2R,16, length*4, width*4);
  SaveImagePgm(NAME_IMG_OUT,MatriceImg2R,length*4,width*4);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);
 


  //==End=========================================================

  //Liberation memoire 
  free_fmatrix_2d(MatriceImg1R);
  free_fmatrix_2d(MatriceImg2R);  
 
  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


