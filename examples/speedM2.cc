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
    memset(in_buff , 123, size_in_buff);
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


    for(i = 0; i < 12; i++){
        printf("+++++++++ %d +++++++++\n", i);
        /* w64 multiply{2} */
        NOTE("w64 muliply{2}");
        gettimeofday(&t_begin , NULL);
        galois_w64_region_multby_2(in_buff, size[i]);
        gettimeofday(&t_end, NULL);
        printf("%04.3f MB/S\t\t(%2.3f MB)\n", (size[i]/1024.0)/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0), size[i]/(1024.0*1024.0));
    
    
        /* w32 multiply{2} */
        NOTE("w32 muliply{2}");
        galois_w32_region_multby_2(in_buff, size[i]);
        gettimeofday(&t_begin , NULL);
        galois_w32_region_multby_2(in_buff, size[i]);
        gettimeofday(&t_end, NULL);
        printf("%04.3f MB/S\t\t(%2.3f MB)\n", (size[i]/1024.0)/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0), size[i]/(1024.0*1024.0));
    
    
        /* w16 multiply{2} */
        NOTE("w16 muliply{2}");
        galois_w16_region_multby_2(in_buff, size[i]);
        gettimeofday(&t_begin , NULL);
        galois_w16_region_multby_2(in_buff, size[i]);
        gettimeofday(&t_end, NULL);
        printf("%04.3f MB/S\t\t(%2.3f MB)\n", (size[i]/1024.0)/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0), size[i]/(1024.0*1024.0));
    
    
        /* w8 multiply{2} */
        NOTE("w8 muliply{2}");
        galois_w08_region_multby_2(in_buff, size[i]);
        gettimeofday(&t_begin , NULL);
        galois_w08_region_multby_2_64(in_buff, size[i]);
        gettimeofday(&t_end, NULL);
        printf("%04.3f MB/S\t\t(%2.3f MB)\n", (size[i]/1024.0)/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0), size[i]/(1024.0*1024.0));
    
    } 

    free(in_buff);
}
