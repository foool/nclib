#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"


/* Test GMatrix64 */
int main(void){
    GMatrix64 mat64;
    GMatrix64 mat64_res;
    GMatrix64 mat64_i;
    GMatrix mat;
/*
    printf("test:"PRIu64"\n", (1UL<<32));
    printf("test:"PRIu64"\n", (1UL<<63));
    
    uint32_t a, b, c;
    a = rand();
    printf("a = %d\n", a);
    b = (uint32_t)galois_shift_inverse(a, 32);
    printf("b = %d\n", b);
    c = galois_single_multiply(a, b, 32);
    printf("%d * %d = %d\n", a, b, c);

    uint64_t x, y, z;
    x = rand();
    x = rand()|(x<<32);

    printf("x = %" PRIx64 "\n", x);
    y = galois_shift_inverse_64(x);
    printf("y = %" PRIx64 "\n", y);
    printf("x * y = %" PRIx64 "\n" , galois_shift_multiply_64(x, y));
    printf("8 * 1 = %" PRIx64 "\n" , galois_shift_multiply_64(8UL, 1UL));
    printf("1 * 8 = %" PRIx64 "\n" , galois_shift_multiply_64(1UL, 8UL));
    printf("8 * 8 = %" PRIx64 "\n" , galois_shift_multiply_64(8UL, 8UL));
    
    NOTE(" Test Make_random and Print ");
    mat64.Make_random(4, 4);
    //mat64.Set(0,3,1UL);
    mat64.Print();
    mat64_i = mat64;
    mat64_i.Inverse();
    NOTE("mat64_i");
    mat64_i.Print();
    mat64_res = Prod(mat64, mat64_i);
    NOTE("mat64 * mat64_i");
    mat64_res.Print();
*/
    /*vandemonde test*/
    mat.Make_vandermonde(4, 4, 8);
    mat64 = Mat8to64(mat);
    NOTE("vandermonde matrix from GF(2^8)");
    mat64.Print();
    for(int i = 1; i < 258; i++){
        mat.Make_vandermonde(i, i, 8);
        mat64 = Mat8to64(mat);
        printf("i = %d\n",i);
        if(Is_full(mat64)){
            NOTE("Full");
        }else{
            NOTE("Not-full");
            mat64.Print();
            exit(-1);
        }
        //mat64.Print();
    }
}
