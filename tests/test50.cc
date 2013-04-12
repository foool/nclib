#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* test ./gfrcfunc 6 4 3 2 1 in ../example */
int main(int argc, char *argv[]){
    int loop;

    loop = 1000;
    while(--loop>=0){
        GMatrix mat, m0, m1, mat_r0, mat_r1, mat_r, mat_t, mat_s, matx, maty;
        mat.Make_random(6, 4, 8);
        while(0 == NK_property(mat, 2, 2)){
            mat.Make_random(6, 4, 8);
        }
        m0 = Slice_matrix(mat, 0, 2);
        m1 = Slice_matrix(mat, 2, 2);
        mat_r0.Make_random(1, 2, 8);
        mat_r1.Make_random(1, 2, 8);
        mat_t = Prod(mat_r0, m0);
        mat_s = Prod(mat_r1, m1);
        matx = mat_t;
        matx.Append_matrix(mat_s);
        matx.Append_matrix(m0);
        if(Rank(matx) != 3){
            printf("error\n");
        }
        maty = mat_t;
        maty.Append_matrix(mat_s);
        maty.Append_matrix(m1);
        if(Rank(maty) != 3){
            printf("error\n");
        }
    }

    return 1;
}
