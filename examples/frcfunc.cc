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

    mat.Make_random(row, col, 16);
    //mat.Make_vandermonde(row, col, 8);
    while(1 != NK_property(mat, row/n, k)){
        mat.Make_random(row, col, 16);
        NOTE("Generate new matrix!");
        //getchar();
    }
    NOTE("Original mat matrix, generate original matrix success");
    mat.Print();
    getchar();

    // loop
    srand(time(NULL));
    //while(count>=0){
    while(true){
        
        // invalid a node 
        fnode = rand()%n;
        //fnode = ++fnode_t%n;
        mat_=mat;
        mat.Wipe_matrix(fnode*alpha, alpha, 0);
        //printf("fail node %d \t\t", fnode);
        //mat.Print();
        //getchar();
        //printf("----------- begin to repair ------------\n");

        //repair
        succ = repair(mat, k, alpha, fnode, beta);
        if(succ){
            //printf("Success to repair\n");
            //mat.Print();
            ++ccc;
            if(ccc%10000 == 0){
                printf("%10d\n",ccc);
            }
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
    int dbeta = d*beta;

    assert(vpn <= alpha);

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

        //mat_rnd.Make_vandermonde(alpha, beta*d, 8);
        mat_rnd.Make_random_nz(alpha, beta*d, 16);
        if(false == Is_full(mat_rnd)){
            continue;
        }
        mat_rpr = Prod(mat_rnd, mat_beta);

        mat_.Replace_matrix(mat_rpr, fnode*alpha, alpha);
        mat__.Replace_matrix(mat_rpr, fnode*alpha, alpha);
        mat__.Clear_zero_rows();

#if 0
        if(true == NK_property_(mat_, alpha, k, beta)){
            printf("any cols true");
        }else{
            printf("any cols false");
        }
        getchar();
#endif
        //if(true == NK_property(mat_, alpha, k)){
        if(true == NK_property(mat_,alpha,k)&&true == DB_property(mat_,ds,beta)){
            //if(false == RP_property(mat_, mat_.rr/alpha, k, beta)){
            //    printf("RP_property false!\n");
            //    getchar();
            //}
            if(false == DB_property(mat_, ds, beta)){
                /*
                printf("DB_property failed !!!!\n");
                NOTE("mat_");
                mat_.Print();
                NOTE("mat_beta");
                mat_beta.Print();
                NOTE("mat_rnd");
                mat_rnd.Print();
                NOTE("mat");
                mat.Print();
                getchar();
                */
            }
            
            mat = mat_;
            return true;
        }else{
            /*
            if(false == DB_property(mat_, ds, beta)){
                printf("DB_property failed !!!!\n");
                NOTE("mat_");
                mat_.Print();
                NOTE("mat_beta");
                mat_beta.Print();
                NOTE("mat_rnd");
                mat_rnd.Print();
                NOTE("mat");
                mat.Print();
                getchar();
            }
            */
            //printf("more than 1 loop");
            //getchar();
        }
    }
    return false;
}
