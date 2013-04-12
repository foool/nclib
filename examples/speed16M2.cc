#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* Speed of GMatrixU8 coding */
int main(void){
    int i;
    ntime t_begin, t_end;
    unsigned char * in_buff;
    unsigned char * out_buff;
    unsigned long size[12];
    unsigned long size_in_buff = 65*1024*1024;
    
    in_buff = (unsigned char *)malloc(size_in_buff);
    memset(in_buff , 153, size_in_buff);
    size[0] = 128*1024;
    size[1] = 256*1024;
    size[2] = 512*1024;
    size[3] = 1024*1024;
    size[4] = 1536*1024;
    size[5] = 2048*1024;
    size[6] = 4096*1024;
    size[7] = 8192*1024;
    size[8] = 16*1024*1024;
    size[9] = 32*1024*1024;
    size[10] = 48*1024*1024;
    size[11] = 64*1024*1024;


    for(i = 3; i < 4; i++){
        int count = 1000; 
        /* w16 multiply{2} */
        printf("w16 muliply{2}(without aligment)\t");
        galois_w16_region_multby_2(in_buff, size[i]);
        gettimeofday(&t_begin , NULL);
        while(--count >= 0){
            galois_w16_region_multby_2(in_buff, size[i]);
        }
        gettimeofday(&t_end, NULL);
        printf("%04.3f MB/S\t\t(%2.3f MB)\n", (size[i]/1.024)/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0), size[i]/(1024.0*1024.0));
        

        count = 1000;
        printf("w16 muliply{2}(aligment)\t\t");
        gettimeofday(&t_begin , NULL);
        while(--count >= 0){
            galois_w16_region_multby_2_64(in_buff, size[i]);
        }
        gettimeofday(&t_end , NULL);
        printf("%04.3f MB/S\t\t(%2.3f MB)\n", (size[i]/1.024)/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0), size[i]/(1024.0*1024.0));
    }
    
    free(in_buff);
}
