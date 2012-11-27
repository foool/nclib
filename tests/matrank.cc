#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"


/* Test Rank() */
int main(void){
    GMatrix m1;
    FILE *fp;


    NOTE(" Read from a file ");
    if(NULL == (fp = fopen("matrix.dat","r"))){
        printf("fail to open file\n");
    }
    m1.Make_from_file(fp, 6, 6, 8);
    m1.Print();

    printf("The rank of matrix is %d\n", Rank(m1));
    fclose(fp);
}
