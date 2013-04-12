#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../nc.h"

/* Speed of memcpy */
int main(void){
    int i;
    uint64_t a, b, c;
    ntime t_begin, t_end;
    unsigned char * in_buff;
    unsigned char * out_buff;
    unsigned char * out_buff2;
    unsigned long size[12];
    unsigned long size_in_buff = 65*1024*1024;
    unsigned long size_out_buff = 65*1024*1024;
    unsigned long size_out_buff2 = 65*1024*1024;
    
    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_out_buff);
    out_buff2 = (unsigned char *)malloc(size_out_buff2);
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

    uint64_t *l1, *l2, *l3, *ltop;
    l1 = (uint64_t *)in_buff;
    l2 = (uint64_t *)out_buff;
    l3 = (uint64_t *)out_buff;
    double dtime;
    
    for(i = 0; i < 12; i++){
        int count = 1024;
        printf("+++++++++ %d +++++++++\n", i);
        gettimeofday(&t_begin , NULL);
        while(--count >= 0){
            memcpy(out_buff, in_buff, size[i]);
        }
        gettimeofday(&t_end, NULL);
        printf("%04.3f MB/S\t\t(%2.3f MB)\n", (size[i]/1.0)/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0), size[i]/(1024.0*1024.0));
    }

    free(in_buff);
    free(out_buff);
    
}
