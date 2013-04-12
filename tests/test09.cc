#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"


/* Test GMatrixU8 */
int main(void){
    int i, j;
    GMatrix mat, mat_i, mat_r;
    GMatrixU8 mat1, mat2, mat3;
    
    for(i = 1; i < 8; i++){
        printf("word = %d\n",i);
        mat1.Make_random(4, 4, i);
        while(!Is_full(mat1)){
            mat1.Make_random(4,4,i);
        }
        mat1.Print();
        NOTE("Inversion of the matrix");
        mat2 = mat1;
        mat2.Inverse();
        mat2.Print();
        mat3 = Prod(mat1, mat2);
        NOTE("mat1 * mat2");
        mat3.Print();
    }

    NOTE("to GMatrix")
    mat = U8tomat(mat1, 8);
    mat_i = Inverse(mat);
    mat_r = Prod(mat, mat_i);
    NOTE("mat matrix");
    mat.Print();
    NOTE("mat_i matrix");
    mat_i.Print();
    NOTE("mat * mat_i");
    mat_r.Print();
}
