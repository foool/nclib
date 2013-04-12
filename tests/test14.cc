#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* Test speed of w*coding_  */
int main(int argc, char *argv[]){
    ntime t_begin, t_end;
    int cm;
    int k;
    int m;
    int count;
    unsigned char * in_buff;
    unsigned char * ot_buff, *ot_buff2;
    unsigned char *pend, *pl;
    unsigned long size_buff = atoi(argv[1]);
    gf_t gfm, gfm8;
    GMatrixU8 mat4, mat4_i, mat4_r;
    GMatrix mat8, mat8_i, mat8_r;
    GMatrix mat16, mat16_i, mat16_r;
    GMatrix mat32, mat32_i, mat32_r;
    GMatrix64 mat64, mat64_i, mat64_r;

    in_buff = (unsigned char *)malloc(size_buff);
    ot_buff = (unsigned char *)malloc(size_buff);
    ot_buff2 = (unsigned char *)malloc(size_buff);
            
    if(size_buff < 1024){
        printf("buffer size: %3f Bytes\n", size_buff*1.0);
    }else{
        if(size_buff < 1024*1024){
            printf("buffer size: %3.2f KB\n", size_buff/1024.0);
        }else{
            printf("buffer size: %04.2f MB\n", size_buff/(1024.0*1024));
        }
    }

    pend = in_buff + size_buff;
    pl = in_buff;
    srand(time(NULL));
    while(pl < pend){
        *pl = rand()%256;
        pl++;
    }

    //mat8.Make_random(4, 4, 8);
    mat8.Make_vandermonde(4, 4, 8);
    NOTE("mat8");
    mat8.Print();

    gettimeofday(&t_begin, NULL);
    count = 100;
    while(--count >=0){
        w8coding_(mat8, ot_buff, in_buff, size_buff);
    }
    gettimeofday(&t_end, NULL);
    printf("Speed(w8coding_):\t%4.3f MB/S\n", (size_buff/(1024.0*1024/100.0))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));

    mat16 = mat8;
    Mat8to16(mat16);
    NOTE("mat16");
    mat16.Print();

    gettimeofday(&t_begin, NULL);
    count = 100;
    while(--count >=0){
        w16coding_(mat16, ot_buff, in_buff, size_buff);
    }
    gettimeofday(&t_end, NULL);
    printf("Speed(w16coding_):\t%4.3f MB/S\n", (size_buff/(1024.0*1024/100.0))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));

    mat16.Make_random(4,4,16);
    NOTE("mat16");
    mat16.Print();

    gettimeofday(&t_begin, NULL);
    count = 100;
    while(--count >=0){
        w16coding_(mat16, ot_buff, in_buff, size_buff);
    }
    gettimeofday(&t_end, NULL);
    printf("Speed(w16coding_):\t%4.3f MB/S\n", (size_buff/(1024.0*1024/100.0))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));


    free(in_buff);
    free(ot_buff);
    free(ot_buff2);
}
