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

int gcd ( int a, int b ){
      int c;
        while ( a != 0 ) {
                 c = a; a = b%a;  b = c;
                   }
          return b;
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
    FILE *fp;

    if(NULL==(fp = fopen("allRC.txt","w"))){
        printf("cann't open allRC.txt\n");
        exit(1);
    }

    fprintf(fp,"   row   col     n     k alpha  beta     d     B\n");
    for(row = 4; row <=99; ++row){
        for(n = 3; n < 50; ++n){
            if(row%n != 0){
                continue;
            }
            alpha = row/n;
            
            for(k = 2; k < n; ++k){
                for(beta = 1; beta < alpha; ++beta){
                    for(col = k*alpha; col >= 2; --col){
                        for(d = n-1; d < n; ++d){
                        
                        //n k property
                        if(alpha*(k-1) >= col){
                            continue;
                        }

                        if(d*beta >= col){
                            continue;
                        }
                        if(gcd(gcd(gcd(gcd(row, col),alpha),beta),d)!=1){
                            continue;
                        }

                        bw = getBW(alpha, k, beta, d);
                        if(bw != col){
                            /* do not satisfy RC porperty */
                            continue;
                        }
                        //add
                        if((n-k)*beta < alpha){
                        //    printf("(%d-%d)*%d < %d\n",n,k,beta,alpha);
                            continue;
                        }
                        //add
                        printf("row:%2d,    col:%2d,    n:%2d,    k:%2d,    alpha:%2d,    beta:%2d,   d:%2d   ,B:%2d)",row, col, n, k, alpha, beta, d, bw);
                        fprintf(fp,"%6d%6d%6d%6d%6d%6d%6d%6d      ",row, col, n, k, alpha, beta, d, bw);
                        getchar();
                        if(beta*d == alpha){
                            printf("MBR\n");
                            fprintf(fp,"MBR\n");
                        }else{
                            if(alpha*k == col){
                                printf("MSR\n");
                                fprintf(fp,"MSR\n");
                                if(alpha >= beta*(n-k+1)){
                                    printf("alpha >= beta*(n-k+1)");
                                    getchar();
                                }
                            }else{
                                printf("\n");
                                fprintf(fp,"\n");
                            }
                        }
                        }
                    }
                }
            }
        }
    }

    fclose(fp);
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
