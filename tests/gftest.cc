#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

void usage(){
    printf("./gftest w methods size iteration\n");
    printf("w : 4 8 16 32 64\n");
    printf("methods:\n");
    printf("0 : region XOR\n");
    printf("1 : region multiply\n");
    printf("2 : region multiply by two\n");
}

/* Test Region multiplication of GF-complete */
int main(int argc, char*argv[]){
    ntime t_begin, t_end;
    unsigned char * in_buff;
    unsigned char * out_buff;
    unsigned char * out_buff2;
    GMatrix mat;
    unsigned long size_in_buff = 64*1024*1024;
    int count;
    int w;
    int meth;
    int iter;
    int val;
    gf_t gfm, gfn, gfw;

    if(argc != 5){
        usage();
        exit(1);
    }else{
        w = atoi(argv[1]);
        meth = atoi(argv[2]);
        size_in_buff = atoi(argv[3]);
        iter = atoi(argv[4]);
        if(w != 4 && w != 8 && w != 16 && w != 32 && w != 64){
            printf("bad w = %d!\n",w);
            exit(1);
        }
        printf("word size(w) = %d\n",w);
        if(meth < 0 || meth >3){
            printf("bad methods!\n");
            exit(1);
        }
        if(iter <= 0){
            printf("Bad iterations!\n");
            exit(1);
        }
        if(size_in_buff <= 0){
            printf("bad size!\n");
            exit(1);
        }else{
            if(size_in_buff < 1024){
                printf("buffer size: %3f Bytes\n", size_in_buff*1.0);
            }else{
                if(size_in_buff < 1024*1024){
                    printf("buffer size: %3.2f KB\n", size_in_buff/1024.0);
                }else{
                    printf("buffer size: %04.2f MB\n", size_in_buff/(1024.0*1024));
                }
            }
        }
        printf("iterations : %d times\n",iter);
    }

    printf("===================================\n");

    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_in_buff);
    out_buff2 = (unsigned char *)malloc(size_in_buff);
    
    //printf(" random set \n");
    unsigned char *pl = in_buff;
    unsigned char *pend;
    pend = pl + size_in_buff;
    srand(time(NULL));
    while(pl < pend){
        *pl = rand()%256;
        pl++;
    }

    if(!gf_init_easy(&gfm, w)){
        printf("Bad gf spec\n");
        exit(1);
    }

    if(w != 32){
        val = rand()%(1U<<w);
    }else{
        val = rand();
    }

    switch(w){
        case 4:
            switch(meth){
                case 0:
                    gfm.multiply_region.w32(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                    galois_region_xor(in_buff, out_buff2, out_buff2, size_in_buff);
                    printf("(w = %d XOR check: )\t", w);
                    if(0 == memcmp(out_buff , out_buff2 , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_region_xor(in_buff, out_buff2, out_buff2, size_in_buff);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w4 XOR, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w32(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w4 XOR, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;
                
                
                case 1:
                    printf("w4 MUL NULL");
                    break;
                
                
                case 2:
                    memcpy(out_buff, in_buff, size_in_buff);
                    galois_w04_region_multby_2(in_buff, size_in_buff);
                    gfm.multiply_region.w32(&gfm, out_buff, out_buff, 2, size_in_buff, 0);
                    //galois_w04_region_multby_2_64(out_buff, size_in_buff);
                    printf("(w = %d X2 check: )\t", w);
                    if(0 == memcmp(out_buff , in_buff , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_w04_region_multby_2_64(out_buff, size_in_buff);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w4 X2, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w32(&gfm, in_buff, in_buff, 2, size_in_buff, 0);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w4 X2, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;
            }
            break;

        /*
         ***************** w = 8 *********************
         */
        case 8:
            switch(meth){
                case 0:
                    gfm.multiply_region.w32(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                    galois_region_xor(in_buff, out_buff2, out_buff2, size_in_buff);
                    printf("(w = %d XOR check: )\t", w);
                    if(0 == memcmp(out_buff , out_buff2 , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_region_xor(in_buff, out_buff2, out_buff2, size_in_buff);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w8 XOR, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w32(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w8 XOR, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;


                case 1:
                    printf("random val : %d\n",val);
                    gfm.multiply_region.w32(&gfm, in_buff, out_buff, val, size_in_buff, 1);
                    galois_w08_region_multiply(in_buff, val, size_in_buff, out_buff2, 1);
                    printf("(w = %d MUL check: )\t", w);
                    if(0 == memcmp(out_buff , out_buff2 , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_w08_region_multiply(in_buff, val, size_in_buff, out_buff2, 1);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w8 MUL, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w32(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w8 MUL, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;


                case 2:
                    memcpy(out_buff, in_buff, size_in_buff);
                    galois_w08_region_multby_2(in_buff, size_in_buff);
                    gfm.multiply_region.w32(&gfm, out_buff, out_buff, 2, size_in_buff, 0);
                    //galois_w08_region_multby_2_64(out_buff, size_in_buff);
                    printf("(w = %d X2 check: )\t", w);
                    if(0 == memcmp(out_buff , in_buff , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_w08_region_multby_2_64(out_buff, size_in_buff);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w8 X2, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w32(&gfm, in_buff, in_buff, 2, size_in_buff, 0);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w8 X2, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;
            }
            break;


        /*
         ***************** w = 16 *********************
         */

        case 16:
            switch(meth){
                case 0:
                    gfm.multiply_region.w32(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                    galois_region_xor(in_buff, out_buff2, out_buff2, size_in_buff);
                    printf("(w = %d XOR check: )\t", w);
                    if(0 == memcmp(out_buff , out_buff2 , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_region_xor(in_buff, out_buff2, out_buff2, size_in_buff);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w16 XOR, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w32(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w16 XOR, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;


                case 1:
                    printf("random val : %d\n",val);
                    gfm.multiply_region.w32(&gfm, in_buff, out_buff, val, size_in_buff, 1);
                    galois_w16_region_multiply(in_buff, val, size_in_buff, out_buff2, 1);
                    printf("(w = %d MUL check: )\t", w);
                    if(0 == memcmp(out_buff , out_buff2 , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_w16_region_multiply(in_buff, val, size_in_buff, out_buff2, 1);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w16 MUL, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w32(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w16 MUL, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;
                
                
                case 2:
                    memcpy(out_buff, in_buff, size_in_buff);
                    galois_w16_region_multby_2(in_buff, size_in_buff);
                    gfm.multiply_region.w32(&gfm, out_buff, out_buff, 2, size_in_buff, 0);
                    //galois_w16_region_multby_2_64(out_buff, size_in_buff);
                    printf("(w = %d X2 check: )\t", w);
                    if(0 == memcmp(out_buff , in_buff , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_w16_region_multby_2(out_buff, size_in_buff);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w16 X2, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w32(&gfm, in_buff, in_buff, 2, size_in_buff, 0);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w16 X2, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;
            }
            break;


        /*
         ***************** w = 32 *********************
         */

        case 32:
            switch(meth){
                case 0:
                    gfm.multiply_region.w32(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                    galois_region_xor(in_buff, out_buff2, out_buff2, size_in_buff);
                    printf("(w = %d XOR check: )\t", w);
                    if(0 == memcmp(out_buff , out_buff2 , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_region_xor(in_buff, out_buff2, out_buff2, size_in_buff);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w32 XOR, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w32(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w32 XOR, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;

                case 1:
                    printf("random val : %d\n",val);
                    gfm.multiply_region.w32(&gfm, in_buff, out_buff, val, size_in_buff, 1);
                    galois_w32_region_multiply(in_buff, val, size_in_buff, out_buff2, 1);
                    printf("(w = %d MUL check: )\t", w);
                    if(0 == memcmp(out_buff , out_buff2 , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_w32_region_multiply(in_buff, val, size_in_buff, out_buff2, 1);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w32 MUL, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w32(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w32 MUL, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;

                case 2:
                    memcpy(out_buff, in_buff, size_in_buff);
                    galois_w32_region_multby_2(in_buff, size_in_buff);
                    gfm.multiply_region.w32(&gfm, out_buff, out_buff, 2, size_in_buff, 0);
                    //galois_w32_region_multby_2_64(out_buff, size_in_buff);
                    printf("(w = %d X2 check: )\t", w);
                    if(0 == memcmp(out_buff , in_buff , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_w32_region_multby_2(out_buff, size_in_buff);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w32 X2, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w32(&gfm, in_buff, in_buff, 2, size_in_buff, 0);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w32 X2, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;
            }
            break;


        /*
         ***************** w = 64 *********************
         */

        case 64:
            switch(meth){
                case 0:
                    gfm.multiply_region.w64(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                    galois_region_xor(in_buff, out_buff2, out_buff2, size_in_buff);
                    printf("(w = %d XOR check: )\t", w);
                    if(0 == memcmp(out_buff , out_buff2 , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_region_xor(in_buff, out_buff2, out_buff2, size_in_buff);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w64 XOR, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w64(&gfm, in_buff, out_buff, 1, size_in_buff, 1);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w64 XOR, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;
                
                case 1:
                    printf("w = 64 MUL NULL");
                    break;
                
                case 2:
                    memcpy(out_buff, in_buff, size_in_buff);
                    galois_w64_region_multby_2(in_buff, size_in_buff);
                    gfm.multiply_region.w64(&gfm, out_buff, out_buff, 2, size_in_buff, 0);
                    //galois_w64_region_multby_2(out_buff, size_in_buff);
                    printf("(w = %d X2 check: )\t", w);
                    if(0 == memcmp(out_buff , in_buff , size_in_buff)){
                        printf("The Same\n");
                    }else{
                        printf("The Different\n");
                    }

                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            galois_w64_region_multby_2(out_buff, size_in_buff);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w64 X2, old):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    count = iter;
                    gettimeofday(&t_begin , NULL);
                        while(--count >= 0){
                            gfm.multiply_region.w64(&gfm, in_buff, in_buff, 2, size_in_buff, 0);
                        }
                    gettimeofday(&t_end, NULL);
                    printf("Speed(w64 X2, gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
                    break;
            }
            break;
    }



    free(in_buff);
    free(out_buff);
    free(out_buff2);
}
