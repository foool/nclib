#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* Test galois_w04_multby_2 and galois_w04_multby_2_64 */
int main(int argc, char*argv[]){
    ntime t_begin, t_end;
    int cm;
    int k;
    int m;
    unsigned char * in_buff;
    unsigned char * ot_buff;
    unsigned char *pend, *pl;
    unsigned long size_buff = 128;
    gf_t gfm;

    in_buff = (unsigned char *)malloc(size_buff);
    ot_buff = (unsigned char *)malloc(size_buff);

    pend = in_buff + size_buff;
    pl = in_buff;
    srand(time(NULL));
    while(pl < pend){
        *pl = rand()%256;
        pl++;
    }
    
    if(!gf_init_easy(&gfm, 4)){
        printf("Bad gf spec\n");
        exit(1);
    }

    memcpy(ot_buff, in_buff, size_buff);
        printf("------------------------------------------\n");
        printf("in_buff:\n");
        mem_print(in_buff, 16, 8);
        printf("ot_buff:\n");
        mem_print(ot_buff, 16, 8);


    galois_w04_region_multby_2_64(in_buff, size_buff);
    //galois_w04_region_multby_2(ot_buff, size_buff);
    gfm.multiply_region.w32(&gfm, ot_buff, ot_buff, 2, size_buff, 0);

    
    if(0 == memcmp(in_buff, ot_buff, size_buff)){
        printf("The same\n");
    }else{
        printf("The different\n");
        printf("------------------------------------------\n");
        printf("in_buff:\n");
        mem_print(in_buff, 16, 8);
        printf("ot_buff:\n");
        mem_print(ot_buff, 16, 8);

    }

    free(in_buff);
    free(ot_buff);
}
