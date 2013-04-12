#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

void Usage(){
    printf("./u8shift row col\n");
}

/* Speed of GMatrixU8 coding_ */
int main(int argc, char *argv[]){
    ntime t_begin, t_end;
    unsigned char * in_buff;
    unsigned char * out_buff;
    unsigned long size_in_buff = 64*1024*1024;
    unsigned long size_out_buff = 64*1024*1024;
    unsigned long size[] = {1024, 4*1024, 16*1024, 64*1024, \
            256*1024, 1024*1024, 4*1024*1024, 16*1024*1024, 64*1024*1024};
    char buff[10];
    int i, j;
    GMatrix64 mat64;
    GMatrix mat8;
    GMatrix mat16;
    GMatrix mat32;
    GMatrixU8 mat;
    int row ;
    int col ;
    int count;
    gf_t gfm;

    if(argc != 3){
        Usage();
        exit(1);
    }
    row = atoi(argv[1]);
    col = atoi(argv[2]);
    if(row <=0 || col <= 0){
        printf("Bad row/col !/n");
        exit(1);
    }

    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_out_buff);
    
    unsigned char *pl = in_buff;
    unsigned char *pend;
    pend = pl+size_in_buff;
    srand(time(NULL));
    while(pl < pend){
        *pl = rand()%256;
        pl++;
    }



for(i = MIN(row, col); i < 8; i++){
    printf("w = %d\n",i);
    mat.Make_vandermonde(row, col, i);
    mat.Print();
    printf("----------------------------------------------------------------------\n");
    mat8 = U8tomat(mat, 8);
    mat16 = U8tomat(mat, 16);
    mat32 = U8tomat(mat, 32);
    mat64 = U8to64(mat);
    printf("           1KB     4KB    16KB    64KB   256KB    1MB     4MB     16MB    64MB\n");
    
    if(!gf_init_easy(&gfm, 8)){
        printf("Bad gf spec\n");
        exit(1);
    }
    printf("w8_   : ");
    for(j = 0; j < 9; j++){
        count = (int)(4*1024*1024/(size[j]/1024));
        gettimeofday(&t_begin , NULL);
        while(--count>=0){
            w8coding_(mat8, out_buff, in_buff, size[j]);
        }
        gettimeofday(&t_end, NULL);
        sprintf(buff, "%4.1f ", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000)));
	printf("%8s",buff);
    }
    printf("\n");
   
    printf("w8_gf : ");
    for(j = 0; j < 9; j++){
        count = (int)(4*1024*1024/(size[j]/1024));
        gettimeofday(&t_begin , NULL);
        while(--count>=0){
            w8coding_opt(mat8, out_buff, in_buff, size[j], gfm);
        }
        gettimeofday(&t_end, NULL);
        sprintf(buff, "%4.1f ", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000)));
	printf("%8s",buff);
    }
    printf("\n");

    if(!gf_init_easy(&gfm, 16)){
        printf("Bad gf spec\n");
        exit(1);
    }
    printf("w16_  : ");
    for(j = 0; j < 9; j++){
        count = (int)(4*1024*1024/(size[j]/1024));
        gettimeofday(&t_begin , NULL);
        while(--count>=0){
            w16coding_(mat16, out_buff, in_buff, size[j]);
        }
        gettimeofday(&t_end, NULL);
        sprintf(buff, "%4.1f ", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000)));
	printf("%8s",buff);
    }
    printf("\n");
   
    printf("w16_gf: ");
    for(j = 0; j < 9; j++){
        count = (int)(4*1024*1024/(size[j]/1024));
        gettimeofday(&t_begin , NULL);
        while(--count>=0){
            w16coding_opt(mat16, out_buff, in_buff, size[j], gfm);
        }
        gettimeofday(&t_end, NULL);
        sprintf(buff, "%4.1f ", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000)));
	printf("%8s",buff);
    }
    printf("\n");

    if(!gf_init_easy(&gfm, 32)){
        printf("Bad gf spec\n");
        exit(1);
    }
    printf("w32_  : ");
    for(j = 0; j < 9; j++){
        count = (int)(4*1024*1024/(size[j]/1024));
        gettimeofday(&t_begin , NULL);
        while(--count>=0){
            w32coding_(mat32, out_buff, in_buff, size[j]);
        }
        gettimeofday(&t_end, NULL);
        sprintf(buff, "%4.1f ", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000)));
	printf("%8s",buff);
    }
    printf("\n");
   
    printf("w32_gf: ");
    for(j = 0; j < 9; j++){
        count = (int)(4*1024*1024/(size[j]/1024));
        gettimeofday(&t_begin , NULL);
        while(--count>=0){
            w32coding_opt(mat32, out_buff, in_buff, size[j], gfm);
        }
        gettimeofday(&t_end, NULL);
        sprintf(buff, "%4.1f ", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000)));
	printf("%8s",buff);
    }
    printf("\n");

    if(!gf_init_easy(&gfm, 64)){
        printf("Bad gf spec\n");
        exit(1);
    }
    printf("w64_  : ");
    for(j = 0; j < 9; j++){
        count = (int)(4*1024*1024/(size[j]/1024));
        gettimeofday(&t_begin , NULL);
        while(--count>=0){
            w64coding_(mat64, out_buff, in_buff, size[j]);
        }
        gettimeofday(&t_end, NULL);
        sprintf(buff, "%4.1f ", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000)));
	printf("%8s",buff);
    }
    printf("\n");
   
    printf("w64_gf: ");
    for(j = 0; j < 9; j++){
        count = (int)(4*1024*1024/(size[j]/1024));
        gettimeofday(&t_begin , NULL);
        while(--count>=0){
            w64coding_opt(mat64, out_buff, in_buff, size[j], gfm);
        }
        gettimeofday(&t_end, NULL);
        sprintf(buff, "%4.1f ", 4*1024.0/((t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000)));
	printf("%8s",buff);
    }
    printf("\n");

}

    free(in_buff);
    free(out_buff);
} 
