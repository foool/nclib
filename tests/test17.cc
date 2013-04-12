#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* test NK_property of GMatrix64 and GMatrixU8 */
int main(int argc, char *argv[]){
    GMatrix mat16, mat16_a, mat16_;
    GMatrix64 mat64;
    GMatrix64 mat64_a, mat64_;
    GMatrixU8 mat8;
    GMatrixU8 mat8_a, mat8_;
    int i = 1;
    int n, k, cols, rows, alpha;
    int rnd;
    int w;

    do{
        //generate random n, k, alpha and cols
        n = NC_random(2,30);
        k = NC_random(1,n);
        alpha = NC_random(1,8);
        cols = NC_random(MAX((k-1)*alpha, 1), n*alpha);
        rows = n*alpha;
        printf("GM: %3d rows, %3d cols %3d alpha(n = %3d, k=%3d)\n",rows,cols,alpha,n,k);

        rnd = NC_random(1,4);
        w = 8*rnd;
        if(w == 24 ){
            w = 32;
        }
        mat16.Make_random(rows, cols, w);
        if(NK_property(mat16, alpha, k)==true){
            printf("%2d -- true\n", w);
        }else{
            printf("%2d -- false\n", w);
        }

        mat64.Make_random(rows, cols);
        if(NK_property(mat64, alpha, k)==true){
            printf("64 -- true\n");
        }else{
            printf("64 -- false\n");
        }

        rnd = NC_random(1,8);
        mat8.Make_random(rows, cols, rnd);
        if(NK_property(mat8, alpha, k)==true){
            printf("%2d -- true\n", rnd);
        }else{
            printf("%2d -- false\n", rnd);
        }
        getchar();
    }while(1);

    return 1;
}
