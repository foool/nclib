#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


void Check_denpendence(GMatrix mat){
    int i, cur, j, w;
    int a[100];
    int k;
    int rank;
    GMatrix mat_chk;
    int n =  mat.rr;
    int piece = 1;

    if(NK_property(mat, 3, 3) == true){
        printf("NK_property satisfy!\n");
    }else{
        printf("NK_property NOT satisfy!\n");
    }

    for(j = 2; j < mat.cc; j++){
        k = j;
        printf("Any %d nodes\n", k);
        getchar();
    /* Any k nodes combine into a new matrix *
     * whose rank must equal its cols        */
    for(i = 0; i < k; i++){
        a[i] = i;
    }
    cur = k-1;
    do{
        if(a[cur]-cur <= n-k){
            for(i = 0; i < k; i++){
                if(0 == i){
                    mat_chk = Slice_matrix(mat, a[i], 1);
                }else{
                    mat_chk.Append_matrix(mat, a[i], 1); 
                }
            }
            
            rank = Rank(mat_chk);
            if(rank != k){
                mat_chk.Print();
                for(w = 0;w < k; ++w){
                    printf("%d  ",a[w]);
                }
                getchar();
            }
            a[cur]++;
            continue;
        }else{
            if(0 == cur){
                break;
            }
            a[--cur]++;
            for(i = 1; i < k - cur; i++){
                a[cur+i]=a[cur]+i;
            }
            if(a[cur] - cur < n - k)
                cur = k - 1;
        }
    }while(1);
    }
}

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

    Check_denpendence(mat);
}

