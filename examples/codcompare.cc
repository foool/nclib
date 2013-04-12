#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"


/* Test galois_region_multiply speed and multiply 2 speed */
int main(void){
    ntime t_begin, t_end;
    unsigned char * in_buff;
    unsigned char * out_buff;
    GMatrix mat;
    GMatrix mat2;
    unsigned long size_in_buff = 1024*1024;
    unsigned long size_out_buff = 1024*1024;

    mat.Make_random(4, 4, 8);
    NOTE("mat");
    mat.Print();
    
    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_out_buff);
/*
    NOTE("1MB*1024 times galois_w16_region_multiply");
    int times = 1024;
    gettimeofday(&t_begin, NULL);
    for(int i = 0 ; i < times ; i++){
        galois_w16_region_multiply(in_buff , 135 , size_in_buff , out_buff, 1);
    } 
    gettimeofday(&t_end, NULL);
    printf("Time used: %ds %dus\n",t_end.tv_sec - t_begin.tv_sec, t_end.tv_usec-t_begin.tv_usec);
    printf("Speed(galois_w32_region_multiply):%4.4fMB/S\n", 1024.0/(dt_us(t_end, t_begin)/(1000.0*1000.0)));
*/
    NOTE("1MB*1024 times galois_w16_region_multiply");
    int times = 1024;
    gettimeofday(&t_begin, NULL);
    for(int i = 0 ; i < times ; i++){
        galois_w32_region_multby_2(in_buff , size_in_buff);
    } 
    gettimeofday(&t_end, NULL);
    printf("Time used: %ds %dus\n",t_end.tv_sec - t_begin.tv_sec, t_end.tv_usec-t_begin.tv_usec);
    printf("Speed(galois_w08_region_multby_2):%4.4fMB/S\n", 1024.0/(dt_us(t_end, t_begin)/(1000.0*1000.0)));

    free(in_buff);
    free(out_buff);
}
