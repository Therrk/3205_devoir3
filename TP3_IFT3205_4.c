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
#define NAME_IMG_IN  "ParcMontRoyal"
#define NAME_IMG_OUT  "ParcMontRoyalExtended"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/


/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
    int add_lines = 30;
    int length,width;
    char BufSystVisuImg[100];

    //Lecture Image 
    float** img=LoadImagePgm(NAME_IMG_IN,&length,&width);
    float ** img_copy = fmatrix_allocate_2d(length + add_lines, width);
    fmatrix_zero(length + add_lines, width, img_copy);
    fmatrix_move(length, width, img, &img_copy[add_lines]);
    // Pré extrapolation
    float moy = 0.0;

    for (int i = 0; i < add_lines;i++) {
        for (int j = 0;j < width;j++) {
            img_copy[i][j] = img_copy[2*add_lines - 1 - i][j];
        }
    }

    SaveImagePgm("dump", img_copy, length + add_lines, width);
    
    /* for (int i = 0;i < add_lines;i++) { */
    /*     for (int j = 0;j < width;j++) { */
            
    /*     } */
    /* } */


    float ** matR = fmatrix_allocate_2d(length + add_lines, width);
    float ** matI = fmatrix_allocate_2d(length + add_lines,width);
    float max_norm;
    for (int iter = 0;iter < 15;iter++) {
        fmatrix_zero(length + add_lines, width, matI);
        fmatrix_move(length + add_lines, width, img_copy, matR);
        DFT(matR, matI, 2*add_lines, width);
        if (iter == 0) {
            max_norm = norm(matR[0][0], matI[0][0]);
        }
        float lambda = 0.01 / pow(2, iter);
        float norm_min = max_norm * lambda;
        threshold(matR, matI, length, width, norm_min);
        IDFT(matR, matI, 2*add_lines, width);

        char buffer[100];
        sprintf(buffer, "monroyal_filtered%d", iter);
        SaveImagePgm(buffer, matR, length, width);
      
        float threshold_white = 255.0;
  
        for (int i = 0;i < add_lines;i++) {
            for (int j = 0;j < width;j++) {
                img_copy[i][j] = matR[i][j];
            }
        }
        for (int i = 0;i < length + add_lines;i++) {
            for (int j = 0;j < width;j++) {
                if (img_copy[i][j] > 255.0) {
                    img_copy[i][j] = 255.0;
                }else if (img_copy[i][j] < 0.0) {
                    img_copy[i][j] = 0.0;
                }
            }
        }
    }
  
    SaveImagePgm(NAME_IMG_OUT, img_copy, length, width);
    return 0;
}
