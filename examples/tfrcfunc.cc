#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

unsigned long ccc = 0;

bool repair(GMatrix& mat, const int& k, const int& alpha, const int& fnode, const int& beta, int* pt);

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

    mat.Make_random(row, col, 8);
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
    int tag[100];       //row <= 100
    for(int i = 0; i < 100; ++i){
        tag[i] = 0;
    }
    
    //while(count>=0){
    while(true){
        // invalid a node 
        fnode = rand()%n;
        //fnode = ++fnode_t%n;
        mat_=mat;
        mat.Wipe_matrix(fnode*alpha, alpha, 0);
        
        /* make all tags 0 in fail node*/
        for(int i = 0; i < alpha; ++i){
            tag[fnode*alpha + i] = 0;
        }
        int * pt = &tag[0];
        /* make all tags 0 in fail node*/
        
        printf("fail node %d \t\t", fnode);
        //printf("----------- begin to repair ------------\n");

        //repair
        succ = repair(mat, k, alpha, fnode, beta, pt);
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

bool repair(GMatrix& mat, const int& k, const int& alpha, const int& fnode, const int& beta, int *tag){
    GMatrix mat_ = mat;
    const int alpha_ = alpha;
    const int ds = mat.rr/alpha;
    int d = ds-1;
    int drows = alpha - beta;
    int circle = 4000;
    int ncur = 0;
    int row;
    int ftag[100];
    int vpn = beta;
    GMatrix mat_rpr;
    int i, j;

    assert(vpn <= alpha);
    //printf("n = %d, alpha = %d, beta = %d, vpn = %d\n",mat.rr/alpha,alpha,beta,vpn);

    // loop 1000 times
    while(--circle > 0){
        for(int i = 0; i < 100; ++i){
            ftag[i] = tag[i];
        }

        GMatrix mat_beta, mat_rnd;
        ncur = 0;
        do{
            if(ncur == fnode){
                ++ncur;
                continue;
            }

            // minimum beta tags
            int a[10];   //k < 10
            for(int i = 0 ; i < beta ; ++i){
                a[i] = ncur*alpha+i;
            }
            for(int i = beta ; i < alpha; ++i){
                for(int j = 0 ; j < beta ; ++j){
                    if(tag[a[j]] > tag[ncur*alpha+i]){
                        a[j] = ncur*alpha+i;
                    }
                }
            }
            //add minimum beta tags
            for(int i = 0 ; i < beta ; ++i){
                ++ftag[a[i]];
            }
                   
            vector<int> va;
            for(int i = 0; i < beta ; ++i){
                va.push_back(a[i]);
            }

            GMatrix mat_t = Draw_rows(mat, va, beta);
            
            if(mat_beta.Empty()){
                mat_beta = mat_t;
            }else{
                mat_beta.Append_matrix(mat_t);
            }
            ++ncur;

            //test output
            //NOTE("mat_beta");
            //mat_beta.Print();
        }while(ncur < ds);
        
        printf("\n");
        NOTE("mat_beta");
        mat_beta.Print();
        getchar();

        mat_rnd.Make_random(alpha, beta*d, 8);
        mat_rpr = Prod(mat_rnd, mat_beta);

        mat_.Replace_matrix(mat_rpr, fnode*alpha, alpha);
        if(1 == NK_property(mat_, alpha, k)){
            if(false == RP_property(mat_, mat_.rr/alpha, k, beta)){
                printf("RP_property false!\n");
                getchar();
            }
                for(int i = 0; i < 100; ++i){
                    tag[i] = ftag[i];
                }
                mat = mat_;
            NOTE("tags");
            for(int i = 0; i < mat.rr; ++i){
                printf("%d ", tag[i]);
            }
                return true;
        }
        NOTE("tags");
        for(int i = 0; i < mat.rr; ++i){
            printf("%d ", tag[i]);
        }
    }

    return false;
}
