#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

void region_xor(   unsigned char *r1,         /* Region 1 */
                          unsigned char *r2,         /* Region 2 */
                          unsigned char *r3,         /* Sum region (r3 = r1 ^ r2) -- can be r1 or r2 */
                          int nbytes)       /* Number of bytes in region */
{
  /* nbytes must be a multiply of uint64_t (8 bytes) */
  uint64_t *l1;
  uint64_t *l2;
  uint64_t *l3;
  uint64_t *ltop;
  unsigned char *ctop;

  ctop = r1 + nbytes;
  ltop = (uint64_t *) ctop;
  l1 = (uint64_t *) r1;
  l2 = (uint64_t *) r2;
  l3 = (uint64_t *) r3;

  while (l1 < ltop) {
    *l3 = ((*l1)  ^ (*l2));
    l1++;
    l2++;
    l3++;
  }
}

/* Speed of XOR */
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
            region_xor(in_buff, out_buff, out_buff, size[i]);
        }
        gettimeofday(&t_end, NULL);
        printf("%04.3f MB/S\t\t(%2.3f MB)\n", (size[i]/1.0)/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0), size[i]/(1024.0*1024.0));
    }


    
/*
    for(i = 0; i < 12; i++){
        printf("+++++++++ %d +++++++++\n", i);
        gettimeofday(&t_begin , NULL);
        galois_region_xor(in_buff, out_buff, size[i]);
        gettimeofday(&t_end, NULL);
        printf("%04.3f MB/S\t\t(%2.3f MB)\n", (size[i]/1024.0)/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0), size[i]/(1024.0*1024.0));
    } 
   */
    /*
    NOTE("===== galois_region_xor 2======");
    l1 = (uint64_t *)in_buff;
    l2 = (uint64_t *)out_buff;
    l3 = (uint64_t *)out_buff2;
    gettimeofday(&t_begin , NULL);
    galois_region_xor(in_buff, out_buff, out_buff, 64*1024*1024UL);
    gettimeofday(&t_end, NULL);
    dtime = ((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0);
    printf("%04.3f ms, %04.3f MB/S\n", dtime, 64.0*1024/dtime);
  */
    

    NOTE("===== galois_region_xor 3======");
    l1 = (uint64_t *)in_buff;
    l2 = (uint64_t *)out_buff;
    l3 = (uint64_t *)out_buff2;
    gettimeofday(&t_begin , NULL);
    galois_region_xor_1k(in_buff, out_buff, out_buff2, 64*1024*1024UL); 
    gettimeofday(&t_end, NULL);
    dtime = ((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0);
    printf("%04.3f ms, %04.3f MB/S\n", dtime, 64.0*1024/dtime);
    


/*
    NOTE("===== xor 1+1======");
    l1 = (uint64_t *)in_buff;
    l2 = (uint64_t *)out_buff;
    l3 = (uint64_t *)out_buff;
    gettimeofday(&t_begin , NULL);
    for(int i = 0; i < 8*1024*1024UL; i++){
         a^b;
    }
    gettimeofday(&t_end, NULL);
    dtime = ((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0);
    printf("%04.3f ms, %04.3f MB/S\n", dtime, 64.0*1024/dtime);
*/
/*    
    NOTE("===== xor 1+n======");
    l1 = (uint64_t *)in_buff;
    l2 = (uint64_t *)out_buff;
    l3 = (uint64_t *)out_buff;
    gettimeofday(&t_begin , NULL);
    for(int i = 0; i < 8*1024; i++){
        for(int j = 0; j < 1024; j++){
            *l2 = (*l1)^(*l2);
            l1++;
        }
    }
    gettimeofday(&t_end, NULL);
    dtime = ((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0);
    printf("%04.3f ms, %04.3f MB/S\n", dtime, 64.0*1024/dtime);
*/
   /* 
    NOTE("===== xor n+n======");
    l1 = (uint64_t *)in_buff;
    l2 = (uint64_t *)out_buff;
    l3 = (uint64_t *)out_buff;
    gettimeofday(&t_begin , NULL);
    for(int i = 0; i < 512*1024; i++){
        *(l3) = (*(l1))^(*(l2));
        *(l3+1) = (*(l1+1))^(*(l2+1));
        *(l3+2) = (*(l1+2))^(*(l2+2));
        *(l3+3) = (*(l1+3))^(*(l2+3));
        *(l3+4) = (*(l1+4))^(*(l2+4));
        *(l3+5) = (*(l1+5))^(*(l2+5));
        *(l3+6) = (*(l1+6))^(*(l2+6));
        *(l3+7) = (*(l1+7))^(*(l2+7));
        *(l3+8) = (*(l1+8))^(*(l2+8));
        *(l3+9) = (*(l1+9))^(*(l2+9));
        *(l3+10) = (*(l1+10))^(*(l2+10));
        *(l3+11) = (*(l1+11))^(*(l2+11));
        *(l3+12) = (*(l1+12))^(*(l2+12));
        *(l3+13) = (*(l1+13))^(*(l2+13));
        *(l3+14) = (*(l1+14))^(*(l2+14));
        *(l3+15) = (*(l1+15))^(*(l2+15));
        l3 = l3 + 16;
        l2 = l2 + 16;
        l1 = l1 + 16;
    }
    gettimeofday(&t_end, NULL);
    dtime = ((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0);
    printf("%04.3f ms, %04.3f MB/S\n", dtime, 64.0*1024/dtime);
    */
/*
    NOTE("===== xor n+n+n======");
    l1 = (uint64_t *)in_buff;
    l2 = (uint64_t *)out_buff;
    l3 = (uint64_t *)out_buff2;
    gettimeofday(&t_begin , NULL);
    for(int i = 0; i < 8*1024; i++){
        for(int j = 0; j < 1024; j++){
            *l3 = (*l1)^(*l2);
            l1++;
            l2++;
            l3++;
        }
    }
    gettimeofday(&t_end, NULL);
    dtime = ((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0);
    printf("%04.3f ms, %04.3f MB/S\n", dtime, 64.0*1024/dtime);
    

    free(in_buff);
    free(out_buff);
    */
}
