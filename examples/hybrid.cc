#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <time.h>


bool repair(GMatrix& mat, const int& alpha, const int& fnode, const int& beta);
void usage(){
    printf("Usage:./hybrid n m alpha beta\n");
}

int main(int argc, char *argv[]){
    int n, m, ds;
    int alpha, beta;
    GMatrix mat;
    GMatrix mat_rnd;
    int count = 0;
    int fnode;
    bool succ = false;

    if(argc != 5){
        usage();
        exit(-1);
    }
    // configuration
    n = atoi(argv[1]);      // num of data device
    m = atoi(argv[2]);      // num of checksum device
    alpha = atoi(argv[3]);  // num of vectors each node
    ds = (m+n)/alpha;       // num of nodes
    beta = atoi(argv[4]);   // num of vectors for repair
    
    assert(0 == n%alpha);
    assert(0 == m%alpha);

    mat.Make_identity(n, n, 8);
    mat_rnd.Make_random(m, n, 8);
    mat.Append_matrix(mat_rnd);
    NOTE("Original mat matrix");
    mat.Print();

    // loop
    srand(time(NULL));
    while(++count < 30){
        
        // invalid a node 
        fnode = rand()%(m/alpha)+(n/alpha);
        //mat.Wipe_matrix(rnd*ds, fnode, 0);
        printf("fail node %d \n", fnode);
        //mat.Print();

        //repair
        succ = repair(mat, alpha, fnode, beta);
        if(succ){
            printf("Success to repair\n");
            mat.Print();
        }else{
            printf("Fail to repair\n");
        }
    }
}

bool repair(GMatrix& mat, const int& alpha, const int& fnode, const int& beta){
    GMatrix mat_ = mat;
    const int alpha_ = alpha;
    const int ds = mat.rr/alpha;
    int circle = 1000;
    int ncur = 0;
    int row;
    int vpn = (beta%(ds-1) == 0)?(beta/(ds-1)):(beta/(ds-1)+1);
    GMatrix mat_rpr;
    int i, j;

    assert(vpn <= alpha);
    // loop 1000 times
    while(--circle > 0){
        GMatrix mat_beta, mat_rnd;
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
                //printf("row = %d\n",row);
                if(mat_beta.Empty()){
                    mat_beta = Slice_matrix(mat, row, 1);
                }else{
                    mat_beta.Append_matrix(mat, row, 1);
                }
            }
            //printf("ncur:%d\n",ncur);
            //mat_beta.Print();
            ++ncur;
            free(vpn_idx);
        }while(ncur < ds);

        // Delete redundant rows
        if(beta != mat_beta.rr){
            int ndel = mat_beta.rr - beta;
            for(int i = 0; i < ndel; ++i){
                mat_beta.Del_row(rand()%mat_beta.rr);
            }
        }
        //NOTE("mat_beta after trim");
        //mat_beta.Print();
        //getchar();
        mat_rnd.Make_random(alpha, beta, 8);
        //mat_rnd.Print();
        mat_rpr = Prod(mat_rnd, mat_beta);
        //NOTE("repair matrix");
        //mat_rpr.Print();
        mat_.Replace_matrix(mat_rpr, fnode*alpha, alpha);
        //printf("Rank of the C matrix: %d\n", Rank(Slice_matrix(mat_, 10, 4)));
        if(1 == NK_property(mat_, alpha, mat_.cc/alpha)){
            printf("%d times to repair\n", 1000-circle);
            mat = mat_;
            return true;
        }
    }

    return false;
}
