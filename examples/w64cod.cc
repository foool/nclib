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
    unsigned char * out_buff2;
    GMatrix64 mat_i;
    GMatrix64 mat_m;
    GMatrix64 mat_res;
    GMatrix mat;
    unsigned long size_in_buff = 256;
    unsigned long size_out_buff = 256;
    int count;

    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_out_buff);
    out_buff2 = (unsigned char *)malloc(size_out_buff);
    
    printf(" random set \n");
    unsigned char *pl = in_buff;
    unsigned char *pend;
    pend = pl+size_in_buff;
    srand(time(NULL));
    while(pl < pend){
        *pl = rand()%256;
        pl++;
    }

#if 0
    mat_m.Make_random(4, 4);
    while(!Is_full(mat_m)){
        printf("New matrix");
        mat_m.Make_random(4, 4);
    }
    mat_i = mat_m;
    mat_i.Inverse();
    //mat.Append_matrix(mat_m);
    NOTE("mat_m");
    mat_m.Print();
    NOTE("mat_i");
    mat_i.Print();
    mat_res = Prod(mat_m, mat_i);
    NOTE("mat_res");
    mat_res.Print();
    
    
    NOTE("w64_coding");
    count = 128;
    gettimeofday(&t_begin , NULL);
    while(--count >=0){
        w64coding_(mat_m, out_buff, in_buff, size_in_buff);
    }
    gettimeofday(&t_end, NULL);
    printf("Speed(w64 coding):%3.3f MB/S\n", 128.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
    /*
    w64coding(mat_i, 64, out_buff2, out_buff, size_in_buff);
    if(0 == memcmp(in_buff , out_buff2 , size_out_buff)){
        printf("The Same\n\n");
    }else{
        printf("The Different\n\n");
    }
    //printf("in_buff:%d%d%d%d, out_buff2:%d%d%d%d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff[0],out_buff[1],out_buff[2],out_buff[3]);
  */

/* Test w4coding */
    GMatrixU8 mat4_m;
    GMatrixU8 mat4_i;
    NOTE("mat");
    mat4_m.Make_random(4, 4, 4);
    mat4_m.Print();
    NOTE("inversion of mat");
    mat4_i = mat4_m;
    mat4_i.Inverse();
    mat4_i.Print();
    NOTE("w4_coding");
    count = 128;
    gettimeofday(&t_begin , NULL);
    while(--count>=0){
        w4coding_(mat4_m, out_buff, in_buff, size_in_buff);
    }
    gettimeofday(&t_end, NULL);
    printf("Speed(w4 coding):%3.3f MB/S\n", 128.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
    /*
    w4coding(mat4_i, 4, out_buff2, out_buff, size_in_buff);
    if(0 == memcmp(in_buff , out_buff2 , size_out_buff)){
        printf("The Same\n\n");
    }else{
        printf("The Different\n\n");
    }
    //printf("in_buff:%d%d%d%d, out_buff2:%d%d%d%d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff2[0],out_buff2[1],out_buff2[2],out_buff2[3]);
*/

/* Test w8coding */
    GMatrix mat8_m;
    GMatrix mat8_i;
    NOTE("mat");
    mat8_m.Make_random(4, 4, 8);
    mat8_m.Print();
    NOTE("inversion of mat");
    mat8_i = Inverse(mat8_m);
    mat8_i.Print();
    NOTE("w8_coding");
    count = 128;
    gettimeofday(&t_begin , NULL);
    while(--count >= 0){
        w8coding_(mat8_m, out_buff, in_buff, size_in_buff);
    }
    gettimeofday(&t_end, NULL);
    printf("Speed(w8 coding):%3.3f MB/S\n", 128.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
    /*
    w8coding(mat8_i, 8, out_buff2, out_buff, size_in_buff);
    if(0 == memcmp(in_buff , out_buff2 , size_out_buff)){
        printf("The Same\n\n");
    }else{
        printf("The Different\n\n");
    }
    //printf("in_buff:%d%d%d%d, out_buff2:%d%d%d%d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff2[0],out_buff2[1],out_buff2[2],out_buff2[3]);
*/

/* Test w16coding */
    GMatrix mat16_m;
    GMatrix mat16_i;
    NOTE("mat");
    mat16_m.Make_random(4, 4, 16);
    mat16_m.Print();
    NOTE("inversion of mat");
    mat16_i = Inverse(mat16_m);
    mat16_i.Print();
    NOTE("w16_coding");
    count = 128;
    gettimeofday(&t_begin , NULL);
    while(--count>=0){
        w16coding_(mat16_m, out_buff, in_buff, size_in_buff);
    }
    gettimeofday(&t_end, NULL);
    printf("Speed(w16 coding):%3.3f MB/S\n", 128.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
    /*
    w16coding(mat16_i, out_buff2, out_buff, size_in_buff);
    if(0 == memcmp(in_buff , out_buff2 , size_out_buff)){
        printf("The Same\n\n");
    }else{
        printf("The Different\n\n");
    }
    //printf("in_buff:%d%d%d%d, out_buff2:%d%d%d%d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff2[0],out_buff2[1],out_buff2[2],out_buff2[3]);
    */
#endif
    /* Test w32coding */
    GMatrix mat32_m;
    GMatrix mat32_i;
    NOTE("mat");
    mat32_m.Make_random(4, 4, 32);
    mat32_m.Print();
    NOTE("inversion of mat");
    mat32_i = Inverse(mat32_m);
    mat32_i.Print();
    NOTE("w32_coding");
    //count = 128;
    //gettimeofday(&t_begin , NULL);
    //while(--count >= 0){
        mem_print(in_buff, 256, 64);
        w32coding_(mat32_m, out_buff, in_buff, size_in_buff);
    //}
    //gettimeofday(&t_end, NULL);
    //printf("Speed(w32 coding):%3.3f MB/S\n", 128.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
    
        mem_print(out_buff, 256, 64);
    w32coding(mat32_i, out_buff2, out_buff, size_in_buff);
        mem_print(out_buff2, 256, 64);
    if(0 == memcmp(in_buff , out_buff2 , size_out_buff)){
        printf("The Same\n\n");
    }else{
        printf("The Different\n\n");
    }
    printf("in_buff:%d%d%d%d, out_buff2:%d%d%d%d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff2[0],out_buff2[1],out_buff2[2],out_buff2[3]);



    free(in_buff);
    free(out_buff);
    free(out_buff2);
}
