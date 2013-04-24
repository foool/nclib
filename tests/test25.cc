#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include "../property.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 * test DB_prperty
 */
void usage(){
    printf("Usage:./test25 row col n k beta\n");
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
    mat.Print();
    while(1 != NK_property(mat, row/n, k) || false == DB_property(mat, n, beta) ){
        mat.Make_random(row, col, 8);
        NOTE("Generate new matrix!");
        mat.Print();
        //getchar();
    }
    NOTE("Original mat matrix, generate original matrix success");
    mat.Print();
    getchar();

}

