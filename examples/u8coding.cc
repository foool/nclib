#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* Speed of GMatrixU8 coding_ */
int main(void){
    ntime t_begin, t_end;
    unsigned char * in_buff;
    unsigned char * out_buff;
    unsigned char * out_buff2;
    GMatrixU8 mat;
    unsigned long size_in_buff = 4*1024*1024;
    unsigned long size_out_buff = 4*1024*1024;
    int i;
    GMatrix64 mat_i;
    GMatrix64 mat_m;
    GMatrix64 mat_res;
    GMatrix mat8_m;
    GMatrix mat8_i;
    GMatrix mat8_res;
    GMatrix mat16_m;
    GMatrix mat16_i;
    GMatrix mat16_res;
    GMatrix mat32_m;
    GMatrix mat32_i;
    GMatrix mat32_res;

    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_out_buff);
    out_buff2 = (unsigned char *)malloc(size_out_buff);
    memset(in_buff , 23, size_in_buff);

for(i = 2; i < 8; i++){
    printf("+++++++++ %d +++++++++\n", i);
    /* w64coding_ */
    mat.Make_random(1, 1, i);
    while(!Is_full(mat)){
        printf("New matrix\n");
        mat.Make_random(1, 1, i);
    }
    /*mat.Set(0,0,1);
    mat.Set(0,1,2);
    mat.Set(1,0,0);
    mat.Set(1,1,1);*/
    mat.Print();

    mat_m = U8to64(mat);
    mat_i = mat_m;
    mat_i.Inverse();
    NOTE("mat_m");
    mat_m.Print();
    NOTE("mat_i");
    mat_i.Print();
    mat_res = Prod(mat_m, mat_i);
    NOTE("mat_res");
    mat_res.Print();

    NOTE("w64_coding_");
    gettimeofday(&t_begin , NULL);
    w64coding_(mat_m, out_buff, in_buff, size_in_buff);
    gettimeofday(&t_end, NULL);
    printf("Speed(w64 coding_):%3.3f MB/S\n", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0));
    w64coding_(mat_i, out_buff2, out_buff, size_in_buff);
    
    if(0 == memcmp(in_buff , out_buff2 , size_out_buff)){
        printf("The Same\n\n");
    }else{
        printf("The Different\n\n");
    }
    printf("in_buff:%d %d %d %d, out_buff2:%d %d %d %d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff2[0],out_buff2[1],out_buff2[2],out_buff2[3]);
   
    
    
    /* w8coding_ */
    NOTE("mat");
    mat8_m = U8tomat(mat, 8);
    mat8_m.Print();
    NOTE("inversion of mat");
    mat8_i = Inverse(mat8_m);
    mat8_i.Print();
    mat8_res = Prod(mat8_m, mat8_i);
    NOTE("mat_res");
    mat8_res.Print();

    NOTE("w8_coding_");
    gettimeofday(&t_begin , NULL);
    w8coding_(mat8_m, out_buff, in_buff, size_in_buff);
    gettimeofday(&t_end, NULL);
    printf("Speed(w8 coding_):%3.3f MB/S\n", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0));
    w8coding_(mat8_i, out_buff2, out_buff, size_in_buff);
    
    if(0 == memcmp(in_buff , out_buff2 , size_out_buff)){
        printf("The Same\n\n");
    }else{
        printf("The Different\n\n");
    }
    printf("in_buff:%d %d %d %d, out_buff2:%d %d %d %d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff2[0],out_buff2[1],out_buff2[2],out_buff2[3]);


    
    /* w16coding_ */
    NOTE("mat");
    mat16_m = U8tomat(mat, 16);
    mat16_m.Print();
    NOTE("inversion of mat");
    mat16_i = Inverse(mat16_m);
    mat16_i.Print();
    mat16_res = Prod(mat16_m, mat16_i);
    NOTE("mat16_res");
    mat16_res.Print();

    NOTE("w16_coding_");
    gettimeofday(&t_begin , NULL);
    w16coding_(mat16_m, out_buff, in_buff, size_in_buff);
    gettimeofday(&t_end, NULL);
    printf("Speed(w16 coding_):%3.3f MB/S\n", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0));
    w16coding_(mat16_i, out_buff2, out_buff, size_in_buff);
    
    if(0 == memcmp(in_buff , out_buff2 , size_out_buff)){
        printf("The Same\n\n");
    }else{
        printf("The Different\n\n");
    }
    printf("in_buff:%d %d %d %d, out_buff2:%d %d %d %d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff2[0],out_buff2[1],out_buff2[2],out_buff2[3]);



    /* w32coding_ */
    NOTE("mat");
    mat32_m = U8tomat(mat,32);
    mat32_m.Print();
    NOTE("inversion of mat");
    mat32_i = Inverse(mat32_m);
    mat32_i.Print();
    mat32_res = Prod(mat32_m, mat32_i);
    NOTE("mat32_res");
    mat32_res.Print();

    NOTE("w32_coding_");
    gettimeofday(&t_begin , NULL);
    w32coding_(mat32_m, out_buff, in_buff, size_in_buff);
    gettimeofday(&t_end, NULL);
    printf("Speed(w32 coding_):%3.3f MB/S\n", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0));
    w32coding_(mat32_i, out_buff2, out_buff, size_in_buff);
    if(0 == memcmp(in_buff , out_buff2 , size_out_buff)){
        printf("The Same\n\n");
    }else{
        printf("The Different\n\n");
    }
    printf("in_buff:%d %d %d %d, out_buff2:%d %d %d %d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff2[0],out_buff2[1],out_buff2[2],out_buff2[3]);
    
    getchar();
}

    free(in_buff);
    free(out_buff);
    free(out_buff2);
}
