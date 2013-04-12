#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


bool repair(GMatrix& mat, const int& alpha, const int& fnode, const int& beta);

void usage(){
    printf("Usage:./frcfunc row col n k\n");
}

int getBW(int alpha, int k, int beta, int d){
    int i;
    int ret = 0;

    for(i = 0; i <= k-1; ++i){
        ret += MIN(alpha, (d - i)*beta);
    }

    return ret;
}

int main(){
    int n, k, row, col;
    int alpha, beta;
    int bw;
    int d;


    for(n = 3; n < row; ++n){
        for(k = 2; k < n; ++k){
            for(row = 4; row <=50; ++row){
            if(row%n != 0){
                continue;
            }
            alpha = row/n;
            
                for(beta = 1; beta < alpha; ++beta){
                    for(col = k*alpha; col >= 2; --col){
                        for(d = k; d < n; ++d){
                            // code Rate > 1/3
                            if(row >= 3*col){
                                continue;
                            }
                            bw = getBW(alpha, k, beta, d);
                            if(bw < col){
                                /* do not satisfy RC porperty */
                                continue;
                            }
                        
                            printf("row:%3d,    col:%3d,    n:%3d,    k:%3d,    alpha:%3d,    beta:%3d,   d:%3d,    BW:%3d)",row, col, n, k, alpha, beta, d, bw);
                            if(beta*(n-1) == alpha){
                                printf("MBR\n");
                           }else{
                                if(alpha*k == col){
                                    printf("MSR\n");
                                }else{
                                    printf("\n");
                                }
                            }
                            getchar();
                        }
                    }
                }
            }
        }
    }

}
