#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

void usage(){
    printf("usage: ./mctest w\n");
    printf("w = 8 32 64");
}

/* Test matrix coding with GF-complete */
int main(void){
    ntime t_begin, t_end;
    unsigned char * in_buff;
    unsigned char * out_buff;
    unsigned char * out_buff2;
    GMatrix mat;
    unsigned long size_in_buff = 64*1024*1024;
    unsigned long size_out_buff = 64*1024*1024;
    int count;
    gf_t gfm, gfn, gfw;

    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_out_buff);
    out_buff2 = (unsigned char *)malloc(size_out_buff);
    
    //printf(" random set \n");
    unsigned char *pl = in_buff;
    unsigned char *pend;
    pend = pl+size_in_buff;
    srand(time(NULL));
    while(pl < pend){
        *pl = rand()%256;
        pl++;
    }


/* Test w8coding */
#if 0
    GMatrix mat8_m;
    GMatrix mat8_i;
    NOTE("mat");
    mat8_m.Make_random(4, 4, 8);
    mat8_m.Print();
    NOTE("inversion of mat");
    mat8_i = Inverse(mat8_m);
    mat8_i.Print();
    NOTE("w8_coding");

    count = 1024;
    gfm = gf_complete_init(8);
    gettimeofday(&t_begin , NULL);
        matrix_coding_opt(mat8_m, out_buff, in_buff, size_in_buff, gfm);
    gettimeofday(&t_end, NULL);
    printf("Speed(w8 coding):%3.3f MB/S\n", 64.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
    matrix_coding_(mat8_i, out_buff2, out_buff, size_in_buff);
    if(0 == memcmp(in_buff , out_buff2 , size_out_buff)){
        printf("The Same\n");
    }else{
        printf("The Different\n");
    }
    printf("in_buff:%d%d%d%d, out_buff2:%d%d%d%d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff2[0],out_buff2[1],out_buff2[2],out_buff2[3]);
#endif


/* Test w16coding */
    GMatrix mat16_m;
    GMatrix mat16_i;
    GMatrix mat16_r;
    NOTE("mat");
    mat16_m.Make_random(4, 4, 16);
    mat16_m.Print();
    NOTE("inversion of mat");
    mat16_i = Inverse(mat16_m);
    mat16_i.Print();
    NOTE("Prod of mat and mat_i");
    mat16_r = Prod(mat16_m, mat16_i);
    mat16_r.Print();
    NOTE("w16_coding");
    count = 1024;
    gfn = gf_complete_init(16);
    printf("coding\n");
    gettimeofday(&t_begin , NULL);
        matrix_coding_opt(mat16_m, out_buff, in_buff, size_in_buff, gfn);
    gettimeofday(&t_end, NULL);
    printf("Speed(w16 coding):%3.3f MB/S\n", 64.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
    matrix_coding_(mat16_i, out_buff2, out_buff, size_in_buff);
    if(0 == memcmp(in_buff , out_buff2 , size_in_buff)){
        printf("The Same\n");
    }else{
        printf("The Different\n");
    }
    printf("in_buff: %d %d %d %d, out_buff2: %d %d %d %d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff2[0],out_buff2[1],out_buff2[2],out_buff2[3]);

#if 0
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
    count = 1024;
    gfw = gf_complete_init(32);
    gettimeofday(&t_begin , NULL);
        matrix_coding_opt(mat32_m, out_buff, in_buff, size_in_buff, gfw);
    gettimeofday(&t_end, NULL);
    printf("Speed(w32 coding):%3.3f MB/S\n", 64.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
    matrix_coding_(mat32_i, out_buff2, out_buff, size_in_buff);
    if(0 == memcmp(in_buff , out_buff2 , size_in_buff)){
        printf("The Same\n");
    }else{
        printf("The Different\n");
    }
    printf("in_buff:%d%d%d%d, out_buff2:%d%d%d%d\n",in_buff[0],in_buff[1],in_buff[2],in_buff[3], out_buff2[0],out_buff2[1],out_buff2[2],out_buff2[3]);
#endif

    free(in_buff);
    free(out_buff);
    free(out_buff2);
}
