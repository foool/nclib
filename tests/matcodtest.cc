#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"


/* Test matrix_coding speed */
int main(void){
    ntime t_begin, t_end;
    unsigned char * in_buff;
    unsigned char * out_buff;
    GMatrix mat;
    GMatrix mat2;
    unsigned long size_in_buff = 4*1024*1024;
    unsigned long size_out_buff = 4*1024*1024;

    mat.Make_random(4, 4, 16);
    mat2.Make_sys_vandermonde(8, 4, 32);
    NOTE("mat");
    mat.Print();
    //NOTE("mat2");
    //mat2.Print();
    
    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_out_buff);

    // 512 KB + mat
    NOTE("matrix coding");
    gettimeofday(&t_begin, NULL);
    matrix_coding(mat, out_buff, in_buff, size_in_buff);
    gettimeofday(&t_end, NULL);
    printf("Time used: %ds %dus\n",t_end.tv_sec - t_begin.tv_sec, t_end.tv_usec-t_begin.tv_usec);
    printf("Speed(4MB 4 rows, 4 columns):%03.4fMB/S\n", 4.0/(dt_us(t_end, t_begin)/(1000.0*1000.0)));

    /*
    // 512 KB + mat2
    gettimeofday(&t_begin, NULL);
    matrix_coding(mat2, out_buff, in_buff, size_in_buff);
    gettimeofday(&t_end, NULL);
    printf("Speed(512KB 8 rows, 4 columns):%03.1fMB/S\n",0.512/(dt_s(t_end, t_begin) + dt_ms(t_end, t_begin)/1000.0 + dt_us(t_end, t_begin)/(1000.0*1000.0)));

    size_in_buff = 10*1024*1024;
    size_out_buff =20*1024*1024;
    in_buff = (unsigned char *)realloc(in_buff, size_in_buff);
    out_buff = (unsigned char *)realloc(out_buff, size_out_buff);
    
    // 10 MB + mat
    gettimeofday(&t_begin, NULL);
    matrix_coding(mat, out_buff, in_buff, size_in_buff);
    gettimeofday(&t_end, NULL);
    printf("Speed(10MB 4 rows, 4 columns):%03.1fMB/S\n", 10.0/(dt_s(t_end, t_begin) + dt_ms(t_end, t_begin)/1000.0 + dt_us(t_end, t_begin)/(1000.0*1000.0)));

    // 10 MB + mat2
    gettimeofday(&t_begin, NULL);
    matrix_coding(mat2, out_buff, in_buff, size_in_buff);
    gettimeofday(&t_end, NULL);
    printf("Speed(10MB 8 rows, 4 columns):%03.1fMB/S\n",10.0/(dt_s(t_end, t_begin) + dt_ms(t_end, t_begin)/1000.0 + dt_us(t_end, t_begin)/(1000.0*1000.0)));

*/
    free(in_buff);
    free(out_buff);
}
