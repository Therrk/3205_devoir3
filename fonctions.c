#include <math.h>
#include <stdio.h>

void rotation(int length, int width, float angle, float ** mat_src, float ** mat_dst) {
    // correction rotation dans la mauvaise direction
    angle = -angle;
    
    // Le code
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {

            int half_width = width/2;
            int half_length = length/2;

            // Les coordonnées centrés
            float x_centered = j - half_width;
            float y_centered = i - half_length;


            // La matrice de rotation est la suivante:


            // | cos(θ) -sin(θ) |
            // | sin(θ)  cos(θ) |
            
            // Puisqu'elle est orthonormale, son inverse est sa transposée

            // Les coordonnées où on devrait prendre les pixels
            float x_src = (x_centered*cos(angle) + y_centered*sin(angle)) + half_width;
            float y_src = (x_centered*-sin(angle) + y_centered*cos(angle)) + half_length;


            int up = (int)floor(y_src);
            int down = (int)ceil(y_src);
            int left = (int)floor(x_src);
            int right = (int)ceil(x_src);

            if (left>=0 && right < width && up >= 0 && down < length) {


                // Les quatre pixels réels qui encadrent notre pixel destination



                // à quel distance somme nous du point entier en haut à gauche?
                float x_off = x_src - (float)left;
                float y_off = y_src - (float)up;

                // interpolation linéaire pour le côté supérieur et inférieur du pixel

                /*           up          */
                /*        +------+       */
                /*   left |  le  | right */
                /*        | pixel|       */
                /*        +------+       */
                /*          down         */
                float down_avg = mat_src[down][left] + x_off * (mat_src[down][right] - mat_src[down][left]);
                float up_avg = mat_src[up][left] + x_off * (mat_src[up][right] - mat_src[up][left]);


                // interpolation entre les deux interpolations. le up, down est
                // inversé, car la composante verticale pointe vers le bas.
                float vert_avg = up_avg + y_off * (down_avg - up_avg);

                mat_dst[i][j]= (int)vert_avg;
            } else {
                mat_dst[i][j]= 0;
            }
        }
    }
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }else{
        return b;
    }
}

int min(int a, int b) {
    if (a < b) {
        return a;
    }else{
        return b;
    }
}

float interpol_lin(float** img, int h, int w, float x_src, float y_src)
{
    int up = (int)floor(y_src);
    up = max(0, up);
    int down = (int)ceil(y_src);
    down = min(h - 1, down);
    int left = (int)floor(x_src);
    left = max(0, left);
    int right = (int)ceil(x_src);
    right = min(w - 1, right);

    float col_up =
        (x_src - left) * img[right][up] +
        (right - x_src) * img[left][up];
    float col_down = 
        (x_src - left) * img[right][down] +
        (right - x_src) * img[left][down];

    float col_vert = (y_src - up) * col_down +
        (down - y_src) * col_up;

    return col_vert;
}

void fmatrix_move(int length, int width, float ** src, float ** dst) {
    for (int i = 0;i < length;i++) {
        for (int j = 0;j < width;j++) {
            dst[i][j] = src[i][j];
        }
    }
}

void fmatrix_module(int length, int width, float ** r, float ** im, float ** dst) {
    for (int i = 0;i < length;i++) {
        for (int j = 0;j < width;j++) {
            float real = r[i][j];
            float ima = im[i][j];
            if (isnan(real) || isnan(ima)) {
                printf("erreur dans module\n");
            }
            dst[i][j] = sqrtf(real * real + ima * ima);
        }
    }
}

void fmatrix_zero(int length, int width, float ** mat) {
    for (int i = 0;i < length;i++) {
        for (int j = 0;j < width;j++) {
            mat[i][j] = 0.0;
        }
    }
}
