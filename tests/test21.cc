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
    
    row = 15;
    n = 5;
    k = 3;
    col = 8;
    alpha = 3;
    beta = 1;

    NOTE(" Read from a file ");
    if(NULL == (fp = fopen("matrix.dat","r"))){
        printf("fail to open file\n");
        return 0;
    }
    NOTE("file opened!");
    mat.Make_from_file(fp, 15, 8, 8);
    mat.Print();
    printf("Rank of matrix: ");
    printf("%d\n",Rank(mat));
    while(1 != NK_property(mat, row/n, k)){
        mat.Make_random(row, col, 8);
        NOTE("Generate new matrix!");
        //getchar();
    }
    NOTE("Original mat matrix, generate original matrix success");
    mat.Print();
    getchar();

    RP_property(mat, n, k, beta);
   /* 
    for(int i = 0; i < n; i++){
        GMatrix mat_ = mat;
        mat_.Del_rows(i*alpha, alpha);
        printf("=== After delete row %d ===\n", i);
        mat_.Print();
        DR_property(mat_, n-1, k, beta);
    }
    */
}

