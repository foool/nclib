#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* test NC_code */
int main(int argc, char *argv[]){
    int n, k;
    GMatrix mat;
    GMatrix mat_k;
    GMatrix mat_ins, mat_res;
    int mm[]= {1,1,0,1};
    int * pi;
    unsigned char * d1, *d2;

    NOTE("Input the row an column of the matrix! 4 2");
    scanf("%d %d", &n, &k);
    while(1){
        if(k > n){
            NOTE("Rows must more than cols !");
            NOTE("Input the row an column of the matrix!");
            scanf("%d %d", &n, &k);
            continue;
        }
        mat.Make_random(n, k, 8);
        mat_k = Slice_matrix(mat, 0, k);
        if(!Is_full(mat_k)){
            continue;
        }else{
            Inverse(mat_ins, mat_k);
            break;
        }
    }
    k=2;
    n=2;
    pi = mm;
    mat.Make_from_list(pi, 2, 2, 8);
    Inverse(mat_ins, mat);
    NOTE("Encode matrix");
    mat.Print();
    NOTE("Inversion of the matrix!");
    mat_ins.Print();
    NOTE("matrix multiply the Inversion of the matrix!");
    mat_res = Prod(mat, mat_ins);
    mat_res.Print();

    d1 = (unsigned char *)malloc((8*k+1)*sizeof(unsigned char));
    d2 = (unsigned char *)malloc((8*n+1)*sizeof(unsigned char));

    memset(d1, 68, 8*k);
    d1[8*k] = '\0';
    d2[8*n] = '\0';
    NOTE("The source, Coding");
    printf("%s\n", d1);
    NC_code(mat, d2, d1, 8*k);
    NOTE("After encode, source");
    printf("%s\n",d1);
    NOTE("After encode, Result!");
    printf("%s\n", d2);
    NC_code(mat_ins, d1, d2, 8*k);
    NOTE("After decode, Result!");
    printf("%s\n", d1);

    return 1;
}
