#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* test Rank() of GMatrix64 and GMatrixU8 */
int main(int argc, char *argv[]){
    GMatrix64 mat64;
    GMatrix64 mat64_i, mat64_r;
    GMatrixU8 mat8;
    GMatrixU8 mat8_i, mat8_r;
    int i = 1;

    do{
        mat64.Make_random(i, i);
        mat64.Print();
        if(Rank(mat64) == mat64.cc){
            printf("=== full rank ===\n");
            mat64_i = mat64;
            mat64_i.Inverse();
            printf("Inversion\n");
            mat64_i.Print();
            mat64_r = Prod(mat64, mat64_i);
            NOTE("Result");
            mat64_r.Print();
        }else{
            NOTE(" Non-full Matrix ");
            mat64_i = mat64;
            mat64_i.Inverse(); 
        }
    
        i++;
    }while(getchar() != 'q');
    
    mat64.Make_identity(4, 4);
    NOTE("Identity Matrix");
    mat64.Print();
    if(Rank(mat64) == mat64.cc){
        printf("Full Matrix!\n");
    }else{
        printf("Non-full Matrix!\n");
    }

    NOTE("SET(1, 1, 0)");
    mat64.Set(1, 1, 0);
    if(Rank(mat64) == mat64.cc){
        printf("Full Matrix!\n");
    }else{
        printf("Non-full Matrix!\n");
    }
    mat64.Print();
    NOTE("Inversion");
    //mat64.Inverse();

    NOTE("Make_random");
    mat8.Make_random(4, 4, 4);
    mat8.Print();

    NOTE("Make_identity");
    mat8.Make_identity(4, 4, 4);
    mat8.Print();

    do{
        mat8.Make_random(4, 4, 4);
    }while(Rank(mat8) == mat8.cc);
    
    NOTE("Non-full matrix");
    mat8.Print();
    mat8.Inverse();

    return 1;
}
