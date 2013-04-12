#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(){
    int n, k, row, col;
    int alpha, beta;
    int bw;
    int d, i;

    for( n = 3 ; n <= 10 ; ++n ){
        for( k = 2 ; k < n ; ++k ){
            for( d = k ; d < n ; ++d ){
                for( i = 0 ; i < k ; ++i ){
                    alpha = d+1+i-k;
                    row = n*alpha;
                    col = k*alpha-(i+i)*i/2;
                    beta = 1;
                    printf("%4d %4d %4d %4d %4d\n", row, col, n, k, beta);
                }
            }
        }
    }


}
