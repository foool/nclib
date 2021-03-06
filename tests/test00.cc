#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"


/* Test Make_*****() and Print***() function */
int main(void){
    GMatrix m1;
    FILE *fp;
    int list[]={123, 123213, 23432, 123243, 231, 124343};
    int *pl;
    int i, j;
    uint64_t elem;
    string str("10 100 100 10 1 2 4 8");

    NOTE(" Test Make_random and Print ");
    NOTE("W = 8");
    m1.Make_random(4, 5, 8);
    m1.Print(0);
    NOTE("W = 16");
    m1.Make_random(4, 5, 16);
    m1.Print(0);
    NOTE("W = 32");
    m1.Make_random(4, 5, 32);
    m1.Print(0);

    NOTE("Test Make_from_string");
    m1.Make_from_string(str, 2, 4, 8);
    m1.Print();

    NOTE("Test Make_vandermande");
    m1.Make_vandermonde(8,8,8);
    m1.Print();

    NOTE(" Read from a file ");
    if(NULL == (fp = fopen("matrix.dat","r"))){
        printf("fail to open file\n");
        return 0;
    }
    NOTE("file opened!");
    m1.Make_from_file(fp, 3, 8, 8);
    m1.Print();
    printf("Rank of matrix: %d",Rank(m1));
    fclose(fp);

    NOTE(" GMatrix from a list ");
    pl = list;
    m1.Make_from_list(pl, 3, 2, 32);
    m1.Print();
    NOTE("Print as a string");
    m1.Print_as_str();

    NOTE(" Make_zero ");
    m1.Make_zero(8, 8, 8);
    m1.Print();

    NOTE(" Make_identity ");
    m1.Make_identity(4, 4, 8);
    m1.Print();

    //new
    printf("prim08 = %u\n",galois_single_multiply((1 << 7), 2, 8));
    printf("prim16 = %u\n",galois_single_multiply((1 << 15), 2, 16));
    printf("prim32 = %u\n",galois_single_multiply((1 << 31), 2, 32));

}
