#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <time.h>


bool repair(GMatrix& mat, const int& alpha, const int& fnode, const int& vpn);

int main(int argc, char *argv[]){
    int n, m, ds;
    int alpha, beta;
    GMatrix mat;
    GMatrix mat_rnd;
    int count = 0;
    int rnd;
    bool succ = false;

    // configuration
    n = 4;      // num of data device
    m = 4;      // num of checksum device
    ds = 4;     // num of nodes
    alpha = 2;  // num of vectors each node
    beta = 3;   // num of vectors for repair
    
    mat.Make_identity(n, n, 8);
    mat_rnd.Make_random(m, n, 8);
    mat.Append_matrix(mat_rnd);
    NOTE("mat matrix");
    mat.Print();

    // loop
    srand(time(NULL));
    while(++count < 30){
        
        // invalid a node 
        rnd = rand()%alpha+2;
        //mat.Wipe_matrix(rnd*ds, alpha, 0);
        printf("fail node %d \n", rnd);
        //mat.Print();

        //repair
        succ = repair(mat, alpha, rnd, 1);
        if(succ){
            printf("Success to repair\n");
            mat.Print();
        }else{
            printf("Fail to repair\n");
        }
        getchar();
    }
}

bool repair(GMatrix& mat, const int& alpha, const int& fnode, const int& vpn){
    GMatrix mat_ = mat;
    int alpha_ = alpha;
    const int ds = mat.rr/alpha;
    int circle = 1000;
    int ncur = 0;
    GMatrix mat_rpr;

    // loop 1000 times
    while(--circle > 0){
        GMatrix mat_beta, mat_rnd;
        ncur = 0;
        do{
            if(ncur == fnode){
                ++ncur;
                continue;
            }
            int rnd = rand()%alpha;
            int row = ncur*alpha + rnd;
            if(mat_beta.Empty()){
                mat_beta = Slice_matrix(mat, row, 1);
            }else{
                mat_beta.Append_matrix(mat, row, 1);
            }
            //mat_beta.Print();
            ++ncur;
        }while(ncur < ds);
        mat_rnd.Make_random(alpha, (ds-1)*vpn, 8);
        //mat_rnd.Print();
        mat_rpr = Prod(mat_rnd, mat_beta);
        mat_.Replace_matrix(mat_rpr, fnode*alpha, alpha);
        if(1 == NK_property(mat_, alpha, 2)){
            printf("%d times to repair\n", 1000-circle);
            mat = mat_;
            return true;
        }
    }

    return false;
}
