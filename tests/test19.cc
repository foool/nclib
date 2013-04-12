#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


bool repair(GMatrix& mat, const int& k, const int& alpha, const int& fnode, const int& beta);

void usage(){
    printf("Usage:./frcfunc row col n k beta\n");
}

int getBW(int alpha, int k, int beta, int d){
    int i;
    int ret = 0;

    for(i = 0; i < k-1; i++){
        ret += MIN(alpha, (d - i)*beta);
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
    //NOTE("any two nodes");
    //NK_property(mat, row/n, k-1);
    getchar();
    while(1 != NK_property(mat, row/n, k)){
        mat.Make_random(row, col, 8);
        NOTE("Generate new matrix!");
        //getchar();
    }
    NOTE("Original mat matrix, generate original matrix success");
    mat.Print();
    getchar();

    // loop
    srand(time(NULL));
    while(++count){
        
        // invalid a node 
        //fnode = rand()%n;
        //fnode = ++fnode_t%n;
        fnode = 1;
        mat.Wipe_matrix(fnode*alpha, alpha, 0);
        printf("fail node %d \n", fnode);
        mat.Print();
        //getchar();
        printf("----------- begin to repair ------------\n");

        //repair
        succ = repair(mat, k, alpha, fnode, beta);
        if(succ){
            printf("Success to repair\n");
            mat.Print();
            getchar();
        }else{
            printf("Fail to repair, at the %d-th time\n", count);
            NOTE("The Matrix cann't repair");
            mat.Print();
            exit(1);
        }
        //getchar();
        printf("----------- another loop -------------\n");
    }
}

bool repair(GMatrix& mat, const int& k, const int& alpha, const int& fnode, const int& beta){
    GMatrix mat_ = mat;
    const int alpha_ = alpha;
    const int ds = mat.rr/alpha;
    int d = ds-1;
    int drows = alpha - beta;
    int circle = 40000;
    int ncur = 0;
    int row;
    int vpn = beta;
    GMatrix mat_rpr;
    int i, j;

    assert(vpn <= alpha);
    printf("n = %d, alpha = %d, beta = %d, vpn = %d\n",mat.rr/alpha,alpha,beta,vpn);

    // loop 1000 times
    while(--circle > 0){
        GMatrix mat_beta, mat_rnd;
        GMatrix mat__ = mat;
        ncur = 0;
        do{
            if(ncur == fnode){
                ++ncur;
                continue;
            }
            int *vpn_idx = (int *)malloc(vpn * sizeof(int));
            memset(vpn_idx, -1, vpn);
            for(i = 0; i < vpn ;i++){
                do{
                    row = ncur*alpha + rand()%alpha;
                    for(j = 0; j < vpn; ++j){
                        if(row == vpn_idx[j]){
                            break;
                        }
                    }
                    if(j >= (vpn-1)){
                        break;
                    }else{
                        continue;
                    }
                }while(1);
                vpn_idx[i] = row;
                if(mat_beta.Empty()){
                    mat_beta = Slice_matrix(mat, row, 1);
                    mat__.Wipe_matrix(row, 1, 0);
                }else{
                    mat_beta.Append_matrix(mat, row, 1);
                    mat__.Wipe_matrix(row, 1, 0);
                }
            }
            ++ncur;
            free(vpn_idx);
        }while(ncur < ds);

        NOTE("matrix from d nodes");
        mat_beta.Print();
        printf("rank is %d\n",Rank(mat_beta));
        //getchar();
        mat_rnd.Make_random(alpha, beta*d, 8);
        NOTE("random matrix");
        mat_rnd.Print();
        printf("rank is %d\n",Rank(mat_rnd));
        //getchar();
        mat_rpr = Prod(mat_rnd, mat_beta);
        NOTE("repair matrix");
        mat_rpr.Print();
        printf("rank is %d\n",Rank(mat_rpr));
        //getchar();

        mat_.Replace_matrix(mat_rpr, fnode*alpha, alpha);
        mat__.Replace_matrix(mat_rpr, fnode*alpha, alpha);
        mat__.Clear_zero_rows();

        NOTE("matrix after repair!");
        mat_.Print();
        //NOTE("mat__ clear all zero rows !");
        //mat__.Print();
        if(1 == NK_property(mat_, alpha, k)){
            //if(true == AnyCols(mat__)){
                printf("%d times to repair\n", 1000-circle);
                mat = mat_;
                return true;
            //}
        }
        //printf("The rank of the matrix is %d\n",Rank(mat_));
        //getchar();
        /*GMatrix mat_b;
        mat_b = mat_;
        mat_b.Del_rows(0, 1);
        NOTE("      l check      ");
        mat_b.Print();
        printf("Rank       %d\n",Rank(mat_b));
        getchar();*/
    }

    return false;
}
