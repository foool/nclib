#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"


/*  */
int main(void){
    GMatrix mat_i;
    GMatrix mat;
    int rank;
    
    for(int i = 2 ; i < 10; ++i){
        printf("== %d ==\n", i);
        mat.Make_vandermonde(i, i, 8);
        mat_i = Inverse(mat);
        NOTE("mat");
        mat.Print();
        NOTE("mat_i");
        mat_i.Print();

        //rank = Rank(mat);
        //printf("Rank of the matrix is %d\n", rank);

        Mat8to16(mat);
        mat_i = Inverse(mat);
        NOTE("mat");
        mat.Print();
        NOTE("mat_i");
        mat_i.Print();
        
        //rank = Rank(mat);
        //printf("Rank of the matrix is %d\n", rank);

        Mat16to32(mat);
        mat_i = Inverse(mat);
        NOTE("mat");
        mat.Print();
        NOTE("mat_i");
        mat_i.Print();
        
        //rank = Rank(mat);
        //printf("Rank of the matrix is %d\n", rank);
        getchar();
    }
}
