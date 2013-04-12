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

int main(int argc, char *argv[]){
    int n, k, row, col;
    int alpha, beta;
    int bw;
    
    if(3 != argc){
        printf("Usage: ./nkfindfrc n k\n");
        exit(1);
    }
    n = atoi(argv[1]);
    k = atoi(argv[2]);

    for(row = 4; row <=30; ++row){
            if(row%n != 0){
                continue;
            }
            alpha = row/n;
            
                for(beta = 1; beta < alpha; ++beta){
                    for(col = k*alpha; col >= 2; --col){
                        // code Rate > 1/3
                        if(row >= 3*col){
                            continue;
                        }
                        bw = getBW(alpha, k, beta, n-1);
                        if(bw < col){
                            /* do not satisfy RC porperty */
                            continue;
                        }
                        
                        printf("row: %3d,    col: %3d,    n: %3d,    k: %3d,    alpha: %3d,    beta: %3d,    BW: %3d)",row, col, n, k, alpha, beta, bw);
                        getchar();
                        if(beta*(n-1) == alpha){
                            printf("MBR\n");
                        }else{
                            if(alpha*k == col){
                                printf("MSR\n");
                            }else{
                                printf("\n");
                            }
                        }

                    }
                }
    }

    /* 
    for(row = 4; row <= 100; ++row){
        for(n = row; n >= 3; --n){
            if(row%n != 0){
                continue;
            }
            alpha = row/n;
            
            for(k = n-1; k >= 2; --k){
                for(col = 2; col <= 100; ++col){
                    if(k*alpha < col){
                        continue; 
                    }
                    if(col <= alpha){
                        continue;
                    }
                    for(beta = 1; beta < alpha; ++beta){
                        if(beta*(n-1) <  alpha){
                            continue;
                        }
                        bw = getBW(alpha, k, beta, n-1);
                        if(bw < alpha){
                            continue;
                        }
                        printf("row: %3d,    col: %3d,    n: %3d,    k: %3d,    alpha: %3d,    beta: %3d,    BW: %3d)", row, col, n, k, alpha, beta, bw);
                        if(beta*(n-1) == alpha){
                            printf("MBR\n");
                        }else{
                            if(alpha*k == col){
                                printf("MSR\n");
                            }else{
                                printf("\n");
                            }
                        }

                    }
                }
                getchar();
            }
        }
    }
    */
}
