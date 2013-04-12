#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


bool repair(GMatrix& mat, const int& alpha, const int& fnode, const int& betai, const int& k);

void usage(){
    printf("Usage:./gfrcfunc row col n k beta\n");
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
    GMatrix mat, mat_e;
    int count = 0;
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

    //mat.Make_random(row, col, 8);
    mat.Make_identity(col, col, 8);
    mat_e.Make_random(row - col, col, 8);
    mat.Append_matrix(mat_e);
    if(1 != NK_property(mat, alpha, k)){
        mat.Make_random(row, col, 8);
    }
    NOTE("Original mat matrix");
    mat.Print();

    // loop
    srand(time(NULL));
    while(true){
        
        // invalid a node 
        fnode = rand()%n;
        mat.Wipe_matrix(fnode*alpha, alpha, 0);
        printf("fail node %d \n", fnode);
        mat.Print();
        //getchar();
        printf("----------- begin to repair ------------\n");

        //repair
        succ = repair(mat, alpha, fnode, beta, k);
        if(succ){
            printf("Success to repair\n");
            mat.Print();
        }else{
            printf("Fail to repair\n");
            exit(1);
        }
        //getchar();
        printf("----------- another loop -------------\n");
    }
}

bool repair(GMatrix& mat, const int& alpha, const int& fnode, const int& beta, const int&k){
    GMatrix mat_ = mat;
    const int alpha_ = alpha;
    const int n = mat.rr/alpha;
    int d = n-1;
    int drows = alpha - beta;
    int circle = 1000000;
    int ncur = 0;
    int row = mat.rr;
    int col = mat.cc;
    int count;
    int vpn = beta;
    GMatrix mat_rpr;
    int i, j;

    assert(vpn <= alpha);
    printf("n = %d, alpha = %d, beta = %d, vpn = %d\n", n, alpha, beta, vpn);

    // loop 1000 times
    while(--circle > 0){
        GMatrix mat_beta, mat_rnd;
        GMatrix mat_each, mat_rnd_each, mat_part;

        ncur = 0;
        count = 0;
        while(count < n-1){
            if(ncur == fnode){
                ++ncur;
                continue;
            }
            mat_part = Slice_matrix(mat, ncur*alpha, alpha);
            //printf("node %d: \n",ncur);
            //mat_part.Print();
            //getchar();
            mat_rnd_each.Make_random(beta, alpha, 8);
            //printf("radom matrix for node %d\n",ncur);
            //mat_rnd_each.Print();
            //getchar();
            mat_each = Prod(mat_rnd_each, mat_part);
            if(count == 0){
                mat_beta = mat_each;
            }else{
                mat_beta.Append_matrix(mat_each);
            }
            //printf("RC node %d: \n",ncur);
            //mat_beta.Print();
            //getchar();
            ++ncur;
            ++count;
        }
        mat_rnd.Make_random(alpha, beta*d, 8);
        mat_rpr = Prod(mat_rnd, mat_beta);
        //NOTE("repair matrix");
        //mat_rpr.Print();

        mat_.Replace_matrix(mat_rpr, fnode*alpha, alpha);
        if(1 == NK_property(mat_, alpha, k)){
            printf("%d times to repair\n", 1000-circle);
            mat = mat_;
            //getchar();
            return true;
        }else{
            NOTE("FAIL to repair");
            mat_.Print();
        }
    }

    return false;
}
