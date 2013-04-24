#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

unsigned long ccc = 0;

bool repair(GMatrix& mat, const int& k, const int& alpha, const int& fnode, const int& beta);

void usage(){
    printf("Usage:./frcfunc row col n k beta\n");
}

int getBW(int alpha, int k, int beta, int d){
    int i;
    int ret = 0;

    for(i = 0; i < k-1; i++){
        ret += min(alpha, (d - i)*beta);
    }
}

int main(int argc, char *argv[]){
    int row, col, n, k;
    int alpha, beta;
    GMatrix mat, mat_;
    int fnode_t=-1;
    int count = 10;
    int fnode;
    bool succ = false;

    if(argc != 6){
        usage();
        exit(-1);
    }
    // configuration
    row  = atoi(argv[1]);
    col  = atoi(argv[2]);
    n    = atoi(argv[3]);
    k    = atoi(argv[4]);
    beta = atoi(argv[5]);
    
    assert(row > n);
    assert(0 == row%n);
    alpha = row/n;
    assert(row > col);
    assert(alpha > beta);
    assert(n > k);
    assert((n-1)*beta >= alpha);

    // loop
    srand(time(NULL));

    mat.Make_random(row, col, 8);
    while(1 != NK_property(mat, row/n, k) || false == CL_property(mat, n, k) ){
        mat.Make_random(row, col, 8);
        NOTE("Generate new matrix!");
        //getchar();
    }
    NOTE("Original mat matrix, generate original matrix success");
    mat.Print();
    getchar();

    //while(count>=0){
    while(true){
        
        // invalid a node 
        fnode = rand()%n;
        //fnode = ++fnode_t%n;
        mat_=mat;
        mat.Wipe_matrix(fnode*alpha, alpha, 0);
        printf("fail node %d \t\t", fnode);
        //mat.Print();
        getchar();
        //printf("----------- begin to repair ------------\n");

        //repair
        succ = repair(mat, k, alpha, fnode, beta);
        if(succ){
            //printf("Success to repair\n");
            //mat.Print();
            ++ccc;
            printf("%10d\n",ccc);
        }else{
            printf("Fail to repair, at the %d-th time\n", ccc);
            NOTE("The Matrix cann't repair");
            printf("Fail node %d\n", fnode);
            mat_.Print();
            return -1;
            //exit(1);
        }
        //getchar();
        //printf("----------- another loop -------------\n");
    }
}

bool repair(GMatrix& mat, const int& k, const int& alpha, const int& fnode, const int& beta){
    GMatrix mat_ = mat;
    const int alpha_ = alpha;
    const int ds = mat.rr/alpha;
    int d = ds-1;
    int drows = alpha - beta;
    int circle = 4000;
    int ncur = 0;
    int row;
    int vpn = beta;
    GMatrix mat_rpr;
    int i, j;

    assert(vpn <= alpha);
    //printf("n = %d, alpha = %d, beta = %d, vpn = %d\n",mat.rr/alpha,alpha,beta,vpn);

    // loop 1000 times
    while(--circle > 0){
        //if(circle%1000 == 0){
            //sleep(1);
        //}
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

        //NOTE("mat_beta after trim");
        //mat_beta.Print();
        //NOTE("mat__");
        //mat__.Print();
        //getchar();
        //mat_rnd.Make_vandermonde(alpha, beta*d, 8);
        mat_rnd.Make_random(alpha, beta*d, 8);
        //NOTE("random matrix");
        //mat_rnd.Print();
        mat_rpr = Prod(mat_rnd, mat_beta);
        //NOTE("repair matrix");
        //mat_rpr.Print();
        //getchar();

        mat_.Replace_matrix(mat_rpr, fnode*alpha, alpha);
        mat__.Replace_matrix(mat_rpr, fnode*alpha, alpha);
        mat__.Clear_zero_rows();

        //NOTE("matrix after repair!");
        //mat_.Print();
        //NOTE("mat__ clear all zero rows !");
        //mat__.Print();
        if(1 == NK_property(mat_, alpha, k)){
            if(false == RP_property(mat_, mat_.rr/alpha, k, beta)){
                printf("\nRP_property false!\n");
                //getchar();
            }
            
            if(false == CL_property(mat_, mat_.rr/alpha, k)){
                printf("CL_property false!\n");
                //getchar();
                continue;
            }
            //if(true == AnyCols(mat__)){
                //printf("%d times to repair\n", 1000-circle);
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
