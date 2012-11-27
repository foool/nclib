#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"


/* Test Swap_rows and Swap_cols */
int main(void){
    GMatrix m1;
    int r1, r2;
    int c1, c2;
    
    NOTE(" Test swap rows ");
    m1.Make_random(5, 5, 8);
    m1.Print();
    do{
        printf("Input two rows:");
        scanf("%d %d",&r1,&r2);
        m1.Swap_rows(r1, r2);
        NOTE(" After exchange ");
        m1.Print();
    }while('q' != getchar());

    NOTE(" Test swap columns ");
    m1.Make_random(5, 5, 16);
    m1.Print();
    do{
        printf("Input two columns:");
        scanf("%d %d",&c1,&c2);
        m1.Swap_cols(c1, c2);
        NOTE(" After exchange ");
        m1.Print();
    }while('q' != getchar());

}
