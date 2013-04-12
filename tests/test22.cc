#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char *argv[]){
    int row, col, n, k;
    int alpha, beta;
    GMatrix mat;
    int fnode_t=-1;
    int count = 0;
    int fnode;
    bool succ = false;
    FILE *fp;
   do{ 
    n = NC_random(4, 8);
    k = NC_random(2, n-1);
    alpha = NC_random(2, 7);
    beta = NC_random(1, min(3, alpha));
    
    row = alpha*n;
    col = NC_random((k-1)*alpha+1, k*alpha);

    printf("n=%d, k=%d, alpha=%d, beta=%d, row=%d, col=%d\n",n,k,alpha,beta,row,col);
    getchar();
    mat.Make_random(row, col, 8);
    mat.Print();
    while(1 != NK_property(mat, row/n, k)){
        mat.Make_random(row, col, 8);
        NOTE("Generate new matrix!");
        //getchar();
    }
    NOTE("Original mat matrix, generate original matrix success");
    mat.Print();
    getchar();

    RP_property(mat, n, k, beta);
#if 0 
    for(int i = 0; i < n; i++){
        GMatrix mat_ = mat;
        mat_.Del_rows(i*alpha, alpha);
        printf("=== After delete row %d ===\n", i);
        mat_.Print();
        DR_property(mat_, n-1, k, beta);
    }
    getchar();
#endif
   }while(1);
}

