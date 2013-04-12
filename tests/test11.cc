#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* Test galois_region_multiply speed and multiply 2 speed */
int main(int argc, char*argv[]){
    ntime t_begin, t_end;
    int cm;
    int k;
    int m;
    unsigned char * in_buff;
    unsigned char * out_buff;
    unsigned char * out_buff2;
    unsigned long size_in_buff;
    unsigned long size_out_buff;
    
    if(argc != 4){
        printf("Usage: ./test11 Coding_Method k m\n");
        printf("Coding_Method\n");
        printf("0: matrix_coding\n");
        printf("1: matrix_coding_(sheduled)\n");
        printf("2: shift_coding\n");
        printf("3: shift_coding(sheduled)\n");
        exit(-1);
    }

    cm = atoi(argv[1]);
    k = atoi(argv[2]);
    m = atoi(argv[3]);
    size_in_buff = k*1024*1024;
    size_out_buff = m*1024*1024;
    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_out_buff);
    out_buff2 = (unsigned char *)malloc(size_out_buff);
    memset(in_buff , 23, size_in_buff);

    printf(" random set \n");
    unsigned char *pl = in_buff;
    unsigned char *pend;
    pend = pl+size_in_buff;
    srand(time(NULL));
    while(pl < pend){
        *pl = rand()%256;
        pl++;
    }

/* Test w8coding */
    GMatrix mat8_m;
    NOTE("mat08");
    mat8_m.Make_random(m, k, 8);
    //mat8_m.Print();
    gettimeofday(&t_begin , NULL);
    switch(cm){
        case 0:
            matrix_coding(mat8_m, out_buff, in_buff, size_in_buff);
            break;
        case 1:
            matrix_coding_(mat8_m, out_buff, in_buff, size_in_buff);
            break;
        case 2:
            w8coding(mat8_m, out_buff, in_buff, size_in_buff);
            break;
        case 3:
            w8coding_(mat8_m, out_buff, in_buff, size_in_buff);
            break;
    }
    gettimeofday(&t_end, NULL);
    printf("Speed(w8 coding):%3.3f MB/S\n", m*1.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
    
    

/* Test w16coding */
    GMatrix mat16_m;
    NOTE("mat16");
    mat16_m.Make_random(m, k, 16);
    //mat16_m.Print();
    gettimeofday(&t_begin , NULL);
    switch(cm){
        case 0:
            matrix_coding(mat16_m, out_buff, in_buff, size_in_buff);
            break;
        case 1:
            matrix_coding_(mat16_m, out_buff, in_buff, size_in_buff);
            break;
        case 2:
            w16coding(mat16_m, out_buff, in_buff, size_in_buff);
            break;
        case 3:
            w16coding_(mat16_m, out_buff, in_buff, size_in_buff);
            break;
    }
    gettimeofday(&t_end, NULL);
    printf("Speed(w16 coding):%3.3f MB/S\n", m*1.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));


/* Test w32coding */
    GMatrix mat32_m;
    NOTE("mat32");
    mat32_m.Make_random(m, k, 32);
    //mat32_m.Print();
    gettimeofday(&t_begin , NULL);
    switch(cm){
        case 0:
            matrix_coding(mat32_m, out_buff, in_buff, size_in_buff);
            break;
        case 1:
            matrix_coding_(mat32_m, out_buff, in_buff, size_in_buff);
            break;
        case 2:
            w32coding(mat32_m, out_buff, in_buff, size_in_buff);
            break;
        case 3:
            w32coding_(mat32_m, out_buff, in_buff, size_in_buff);
            break;
    }
    gettimeofday(&t_end, NULL);
    printf("Speed(w32 coding):%3.3f MB/S\n", m*1.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));


/* Test w64coding */
    GMatrix64 mat_m;
    mat_m.Make_random(m, k);
    NOTE("mat64");
    //mat_m.Print();
    gettimeofday(&t_begin , NULL);
    switch(cm){
        case 0:
            printf(">>>>w64 has no matrix coding\n");
            exit(-1);
            break;
        case 1:
            printf(">>>>w64 has no matrix coding\n");
            exit(-1);
            break;
        case 2:
            w64coding(mat_m, out_buff, in_buff, size_in_buff);
            break;
        case 3:
            w64coding_(mat_m, out_buff, in_buff, size_in_buff);
            break;
    }
    gettimeofday(&t_end, NULL);
    printf("Speed(w64 coding):%3.3f MB/S\n", m*1.0/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
  

    free(in_buff);
    free(out_buff);
    free(out_buff2);
}
