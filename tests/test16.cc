#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* test NK_property of GMatrix64 and GMatrixU8 */
int main(int argc, char *argv[]){
    GMatrix mat16, mat16_a, mat16_;
    GMatrix64 mat64;
    GMatrix64 mat64_a, mat64_;
    GMatrixU8 mat8;
    GMatrixU8 mat8_a, mat8_;
    int i = 1;
    int n, k, cols, rows, alpha;

    do{
        //generate random n, k, alpha and cols
        n = NC_random(2,30);
        k = NC_random(1,n);
        alpha = NC_random(1,8);
        cols = NC_random(MAX((k-1)*alpha, 1), n*alpha);
        rows = n*alpha;
        printf("GM: %3d rows, %3d cols %3d alpha(n = %3d, k=%3d)\n",rows,cols,alpha,n,k);

#if 0
        mat16.Make_random(rows, cols, 16);
        if(NK_property(mat16, alpha, k)==true){
            //Check each node
            NOTE("Check each node");
            for(i = 0 ; i < n ; i++){
                mat16_a = Slice_matrix(mat16, i*alpha, alpha);
                if(Is_full(mat16_a)== false){
                    NOTE("error: Matrix(each node)");
                    mat16.Print();
                    NOTE("Non-full Matrix(each node)");
                    mat16_a.Print();
                    exit(-1);
                }
            }

            //Check any k nodes
            NOTE("Check any k nodes");
            int count = 10;
            while(count > 0){
                mat16_ = mat16;
                //NOTE("MAT64 1");
                //mat16_.Print();
                //Delete any (n-k) nodes of vectors
                for(i = 0; i < (n-k); i++){
                    int bi = NC_random(mat16_.rr/alpha);
                    if(bi+alpha > mat16_.rr){
                        printf("mat16_.rr = %3d, bi = %3d\n",mat16_.rr, bi);
                        exit(-1);
                    }
                    mat16_.Del_rows(bi , alpha);
                }
                //NOTE("MAT64 2");
                //mat64_.Print();
                int rank = Rank(mat16_);
                int min = MIN(mat16_.cc, mat16_.rr);
                //if(rank != min){
                if(Rank(mat16_) != MIN(mat16_.cc, mat16_.rr)){
                    printf("rank is %d,  rr=%d  cc=%d\n",Rank(mat16_), mat16_.rr, mat16_.cc);
                    getchar();
                    NOTE("error: Matrix(any k nodes)");
                    mat16.Print();
                    NOTE("Non-full Matrix(any k nodes)");
                    mat16_.Print();
                    exit(-1);
                    
                }
                count-- ;
            }
        }else{
            continue;
        }
#endif
        mat64.Make_random(rows, cols);
        if(NK_property(mat64, alpha, k)==true){
            //Check each node
            NOTE("Check each node --64");
            for(i = 0 ; i < n ; i++){
                mat64_a = Slice_matrix(mat64, i*alpha, alpha);
                if(Is_full(mat64_a)== false){
                    NOTE("error: Matrix");
                    mat64.Print();
                    NOTE("Non-full Matrix");
                    mat64_a.Print();
                    exit(-1);
                }
            }

            //Check any k nodes
            NOTE("Check any k nodes ---64");
            int count = 100;
            while(count > 0){
                mat64_ = mat64;
                //NOTE("MAT64 1");
                //mat64_.Print();
                //Delete any (n-k) nodes of vectors
                for(i = 0; i < (n-k); i++){
                    int bi = NC_random(mat64_.rr/alpha);
                    if(bi+alpha > mat64_.rr){
                        printf("mat64_.rr = %3d, bi = %3d\n",mat64_.rr, bi);
                        exit(-1);
                    }
                    mat64_.Del_rows(bi , alpha);
                }
                //NOTE("MAT64 2");
                //mat64_.Print();
                int rank = Rank(mat64_);
                int min = MIN(mat64_.cc, mat64_.rr);
                //if(rank != min){
                if(Rank(mat64_) != MIN(mat64_.cc, mat64_.rr)){
                    printf("rank is %d,  MIN =%d\n",Rank(mat64_), MIN(mat64_.rr, mat64_.cc));
                    NOTE("error: Matrix");
                    mat64.Print();
                    NOTE("Non-full Matrix");
                    mat64_.Print();
                    exit(-1);
                    
                }
                count-- ;
            }
        }else{
            continue;
        }

        mat8.Make_random(rows, cols, NC_random(1,8));
        if(NK_property(mat8, alpha, k)==true){
            //Check each node
            NOTE("Check each node --8");
            for(i = 0 ; i < n ; i++){
                mat8_a = Slice_matrix(mat8, i*alpha, alpha);
                if(Is_full(mat8_a)== false){
                    NOTE("error: Matrix");
                    mat8.Print();
                    NOTE("Non-full Matrix");
                    mat8_a.Print();
                    exit(-1);
                }
            }

            //Check any k nodes
            NOTE("Check any k nodes ---8");
            int count = 100;
            while(count > 0){
                mat8_ = mat8;
                //NOTE("MAT64 1");
                //mat64_.Print();
                //Delete any (n-k) nodes of vectors
                for(i = 0; i < (n-k); i++){
                    int bi = NC_random(mat8_.rr/alpha);
                    if(bi+alpha > mat8_.rr){
                        printf("mat8_.rr = %3d, bi = %3d\n",mat8_.rr, bi);
                        exit(-1);
                    }
                    mat8_.Del_rows(bi , alpha);
                }
                //NOTE("MAT64 2");
                //mat64_.Print();
                int rank = Rank(mat8_);
                int min = MIN(mat8_.cc, mat8_.rr);
                //if(rank != min){
                if(Rank(mat8_) != MIN(mat8_.cc, mat8_.rr)){
                    printf("rank is %d,  MIN =%d\n",Rank(mat8_), MIN(mat8_.rr, mat8_.cc));
                    NOTE("error: Matrix");
                    mat8.Print();
                    NOTE("Non-full Matrix");
                    mat8_.Print();
                    exit(-1);
                    
                }
                count-- ;
            }
        }else{
            continue;
        }

    }while(1);

    return 1;
}
